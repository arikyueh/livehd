//  This file is distributed under the BSD 3-Clause License. See LICENSE for details.

#include "mmap_vector.hpp"

#include "lnast.hpp"

void Lnast_node::dump() const {
  fmt::print("type:{}\n", type.debug_name()); // TODO: cleaner API to also dump token
}


Lnast::~Lnast() {
  for (auto *ptr : string_pool) delete ptr;

  if (memblock_fd==-1)
    return;

  int ok = ::munmap((void *)memblock.data(), memblock.size());
  I(ok==0);
  close(memblock_fd);

  memblock = "";
  memblock_fd = -1;
}

std::string_view Lnast::add_string(std::string_view sview) {
  string_pool.emplace_back(new std::string(sview));

  auto *str_ptr = string_pool.back();

  return *str_ptr;
}

std::string_view Lnast::add_string(const std::string &str) {
  string_pool.emplace_back(new std::string(str));

  auto *str_ptr = string_pool.back();

  return *str_ptr;
}

void Lnast::do_ssa_trans(const Lnast_nid &top_nid) {

  Lnast_nid top_sts_nid;
  if (get_type(top_nid).is_func_def()) {
    /* fmt::print("Step-0: Handle Inline Function Definition\n"); */
    auto c0 = get_first_child(top_nid);
    auto c1 = get_sibling_next(c0);
    top_sts_nid = get_sibling_next(c1);
  } else {
    top_sts_nid = get_first_child(top_nid);
  }


  Phi_rtable top_phi_resolve_table;
  phi_resolve_tables[top_sts_nid] = top_phi_resolve_table;

  /* fmt::print("Step-1: Analyze LHS or RHS of Tuple Dot/Sel\n"); */
  analyze_dot_lrhs(top_sts_nid);

  /* fmt::print("Step-2: Tuple_Add/Tuple_Get Analysis\n"); */
  trans_tuple_opr(top_sts_nid);

  /* fmt::print("Step-3: LHS SSA\n"); */
  resolve_ssa_lhs_subs(top_sts_nid);

  //see Note I
  /* fmt::print("Step-4: RHS SSA\n"); */
  resolve_ssa_rhs_subs(top_sts_nid);


  /* fmt::print("Step-5: Operator LHS Merge\n"); */
  opr_lhs_merge(top_sts_nid);


  /* fmt::print("LNAST SSA Transformation Finished!\n"); */
}


void Lnast::trans_tuple_opr(const Lnast_nid &psts_nid) {
  Tuple_var_table top_tuple_var_table;
  tuple_var_tables[psts_nid] = top_tuple_var_table;
  for (const auto &opr_nid : children(psts_nid)) {
    auto type = get_type(opr_nid);
    if (type.is_func_def()) {
      continue;
    } else if (type.is_if()) {
      trans_tuple_opr_if_subtree(opr_nid);
    } else if (type.is_tuple()) {
      rename_to_real_tuple_name(psts_nid, opr_nid);
      update_tuple_var_table(psts_nid, opr_nid);
    } else if (is_attribute_related(opr_nid)) {
      auto dot_nid = opr_nid;
      dot2attr_set_get(psts_nid, dot_nid);
      update_tuple_var_table(psts_nid, opr_nid);
    } else if (type.is_tuple_concat()) {
      merge_tconcat_paired_assign(psts_nid, opr_nid);
    } else if (type.is_dot() || type.is_select()) {
      trans_tuple_opr_handle_a_statement(psts_nid, opr_nid);
    } else {
      update_tuple_var_table(psts_nid, opr_nid);
    }
  }
}


void Lnast::trans_tuple_opr_if_subtree(const Lnast_nid &if_nid) {
  for (const auto &itr_nid : children(if_nid)) {
    if (get_type(itr_nid).is_stmts()) {
      Tuple_var_table if_sts_tuple_var_table;
      tuple_var_tables[itr_nid] = if_sts_tuple_var_table;

      for (const auto &opr_nid : children(itr_nid)) {
        auto type = get_type(opr_nid);
        I(!type.is_func_def());
        if (type.is_if()) {
          trans_tuple_opr_if_subtree(opr_nid);
        } else if (is_attribute_related(opr_nid)) {
          auto dot_nid = opr_nid;
          dot2attr_set_get(itr_nid, dot_nid);
          update_tuple_var_table(itr_nid, opr_nid);
        } else if (type.is_dot() || type.is_select()) {
          trans_tuple_opr_handle_a_statement(itr_nid, opr_nid);
        } else if (type.is_tuple()) {
          rename_to_real_tuple_name(itr_nid, opr_nid);
          update_tuple_var_table(itr_nid, opr_nid);
        } else {
          update_tuple_var_table(itr_nid, opr_nid);
        }
      }
    }
  }
}

void Lnast::update_tuple_var_table(const Lnast_nid &psts_nid, const Lnast_nid &opr_nid) {
  auto &tuple_var_table = tuple_var_tables[psts_nid];
  auto type = get_type(opr_nid);

  if (type.is_tuple() || type.is_tuple_add()) {
    auto lhs_nid = get_first_child(opr_nid);
    const auto lhs_name = get_name(lhs_nid);
    tuple_var_table.insert(lhs_name);
  }

  return;
}

bool Lnast::is_attribute_related(const Lnast_nid &opr_nid) {
  if (get_type(opr_nid).is_dot()) {
    auto c0_dot  = get_first_child(opr_nid);
    auto c1_dot  = get_sibling_next(c0_dot);
    auto c2_dot  = get_sibling_next(c1_dot);
    auto c2_name = get_name(c2_dot);

    if (c2_name.substr(0,2) == "__" && c2_name.substr(0,3) != "___" )
      return true;
  }
  return false;
}


/**********************************
  LNAST attr_bits merge from dot and assign lnast nodes and create Attr_set, if is dot but at rhs, just

 original:
     dot               assign
    / | \               /  \
   /  |  \             /    \
  /   |   \           /      \
 ___t $a  __bits    ___t     0d4


 merged:
   Attr_set           invalid
    / | \               /  \
   /  |  \             /    \
  /   |   \           /      \
 $a __bits 0d4      ___t     0d4
*/

void Lnast::dot2attr_set_get(const Lnast_nid &psts_nid, Lnast_nid &dot_nid) {
  auto &dot_lrhs_table   = dot_lrhs_tables[psts_nid];
  auto paired_assign_nid = dot_lrhs_table[dot_nid].second;

  auto c0_dot = get_first_child(dot_nid);
  auto c1_dot = get_sibling_next(c0_dot);
  auto c2_dot = get_sibling_next(c1_dot);

  if (is_lhs(psts_nid, dot_nid)) {
    // change node semantic from dot->attr_set ; assign->invalid
    auto c0_assign = get_first_child(paired_assign_nid);
    auto c1_assign = get_sibling_next(c0_assign);
    ref_data(c0_dot)->token = get_data(c1_dot).token;
    ref_data(c1_dot)->token = get_data(c2_dot).token;
    ref_data(c2_dot)->token = get_data(c1_assign).token;
    ref_data(c2_dot)->type  = get_data(c1_assign).type;
    ref_data(dot_nid)->type = Lnast_ntype::create_attr_set();
    ref_data(paired_assign_nid)->type = Lnast_ntype::create_invalid();
  } else { // is rhs
    // change node semantic from dot->attr_get
    ref_data(dot_nid)->type = Lnast_ntype::create_attr_get();
  }
}


void Lnast::merge_tconcat_paired_assign(const Lnast_nid &psts_nid, const Lnast_nid &concat_nid) {
  auto &dot_lrhs_table   = dot_lrhs_tables[psts_nid];
  auto c0_concat = get_first_child(concat_nid);
  auto paired_assign_nid = dot_lrhs_table[concat_nid].second;
  auto c0_assign = get_first_child(paired_assign_nid);
  ref_data(c0_concat)->token = get_data(c0_assign).token;
  ref_data(c0_concat)->type  = get_data(c0_assign).type;
  ref_data(c0_concat)->subs  = get_data(c0_assign).subs;
  ref_data(paired_assign_nid)->type = Lnast_ntype::create_invalid();
}


void Lnast::rename_to_real_tuple_name(const Lnast_nid &psts_nid, const Lnast_nid &tup_nid) {
  auto &dot_lrhs_table   = dot_lrhs_tables[psts_nid];
  auto paired_assign_nid = dot_lrhs_table[tup_nid].second;
  if (paired_assign_nid == Lnast_nid(-1, -1))
    return;

  if (get_type(paired_assign_nid).is_func_call())
    return;


  auto c0_tup            = get_first_child(tup_nid);
  auto c0_paired_assign  = get_first_child(paired_assign_nid);
  ref_data(paired_assign_nid)->type = Lnast_ntype::create_invalid();

  ref_data(c0_tup)->token = get_data(c0_paired_assign).token;
  ref_data(c0_tup)->type  = get_data(c0_paired_assign).type;
  ref_data(c0_tup)->subs  = get_data(c0_paired_assign).subs;
}


void Lnast::trans_tuple_opr_handle_a_statement(const Lnast_nid &psts_nid, const Lnast_nid &opr_nid) {
  I(get_type(opr_nid).is_dot() || get_type(opr_nid).is_select());

  auto dot_nid     = opr_nid;
  auto c0_dot      = get_first_child(dot_nid); //c0 = intermediate target
  auto c1_dot_name = get_name(get_sibling_next(c0_dot));

  if (get_parent(psts_nid) == get_root()) {
    dot2local_tuple_chain(psts_nid, dot_nid);
  } else if (c1_dot_name.substr(0,3) != "___" && check_tuple_table_parents_chain(psts_nid, c1_dot_name)) {
      Lnast_nid cond_nid(-1,-1);
      bool  is_else_sts = false;
      find_cond_nid(psts_nid, cond_nid, is_else_sts);
      dot2hier_tuple_chain(psts_nid, dot_nid, cond_nid, is_else_sts);
  } else { // @if-subtree statements node
      dot2local_tuple_chain(psts_nid, dot_nid);
  }
}


void Lnast::find_cond_nid(const Lnast_nid &psts_nid, Lnast_nid &cond_nid, bool &is_else_sts) {
  if (get_type(psts_nid).is_stmts()) {
    auto prev_sib_nid = get_sibling_prev(psts_nid);
    if (get_type(prev_sib_nid).is_cond()) {
      cond_nid = get_sibling_prev(psts_nid);
      is_else_sts = false;
    } else {
      I(get_type(get_sibling_prev(prev_sib_nid)).is_cond());
      cond_nid = get_sibling_prev(prev_sib_nid);
      is_else_sts = true;
    }
  }
}

void Lnast::dot2local_tuple_chain(const Lnast_nid &psts_nid, Lnast_nid &dot_nid) {
  auto &tuple_var_table = tuple_var_tables[psts_nid];
  auto &dot_lrhs_table  =  dot_lrhs_tables[psts_nid];


  auto paired_nid = dot_lrhs_table[dot_nid].second;
  Lnast_ntype paired_type;
  if (!paired_nid.is_invalid())
    paired_type = get_type(paired_nid);

  if (is_lhs(psts_nid, dot_nid) && paired_type.is_assign()) {
    // merge the tuple_add at the original assign node
    ref_data(dot_nid)->type    = Lnast_ntype::create_invalid();
    ref_data(paired_nid)->type = Lnast_ntype::create_tuple_add();
    auto c0_assign      = get_first_child(paired_nid);
    auto c1_assign      = get_sibling_next(c0_assign);
    auto c1_assign_data = get_data(c1_assign);

    int i = 0;
    for (auto child : children(dot_nid)) {
      if (i == 0) {
        i++;
        continue;
      } else if (i == 1) {
        ref_data(c0_assign)->token = get_data(child).token;
        ref_data(c0_assign)->type  = get_data(child).type;
        ref_data(c0_assign)->subs  = get_data(child).subs;
      } else if (i == 2) {
        ref_data(c1_assign)->token = get_data(child).token;
        ref_data(c1_assign)->type  = get_data(child).type;
        ref_data(c1_assign)->subs  = get_data(child).subs;
      } else {
        add_child(paired_nid, get_data(child));
      }
      i++;
    }
    add_child(paired_nid, c1_assign_data);

    auto c0_ta_name = get_name(get_first_child(paired_nid));
    tuple_var_table.insert(c0_ta_name); //insert new tuple name
    return;
  } 


  if (is_lhs(psts_nid, dot_nid) && !paired_type.is_assign()) {
    ref_data(dot_nid)->type = Lnast_ntype::create_tuple_add();
    auto c0_paired = get_first_child(paired_nid);

    auto new_tup_add = insert_next_sibling(paired_nid, get_data(dot_nid));
    

    for (auto dot_child : children(dot_nid)) {
      if (dot_child == get_first_child(dot_nid)){
        continue;
      } else {
        add_child(new_tup_add, get_data(dot_child));
      }
    }
    add_child(new_tup_add, get_data(c0_paired)); //add final child of the new tup_add

    ref_data(dot_nid)->type = Lnast_ntype::create_invalid();

    auto c0_dot_name = get_name(get_first_child(dot_nid));
    tuple_var_table.insert(c0_dot_name); //insert new tuple name
    return;
  } 
  
  // is rhs
  // change node semantic from dot/set->tuple_get
  if (paired_type.is_assign()) {
    ref_data(dot_nid)->type = Lnast_ntype::create_tuple_get();
    auto c0_tg     = get_first_child(dot_nid);
    auto c0_assign = get_first_child(paired_nid);
    ref_data(c0_tg)->token = get_data(c0_assign).token;
    ref_data(c0_tg)->type  = get_data(c0_assign).type;
    ref_data(c0_tg)->subs  = get_data(c0_assign).subs;

    ref_data(paired_nid)->type = Lnast_ntype::create_invalid();
  } else {
    ref_data(dot_nid)->type = Lnast_ntype::create_tuple_get();
  }
}

/*******
  LNAST dot/sel/ and assign node semantic illustration for type replacement

     dot                                       tuple_add
    / | \                                     /    |    \
   /  |  \                                   /     |     \
  c0  c1  c2                                c0     c1     c2
  c0 = temporary target of dot/sel          c0 = tuple name
  c1 = tuple name                           c1 = tuple field
  c2 = tuple field                          c2 = value of tuple field

    assign           tuple_phi_add             tuple_get
    /    \              /     \               /    |    \
   c0    c1            /       \             /     |     \
   c0 = lhs           phi      tuple_add    c0     c1     c2
   c1 = rhs         / | | \      ...        c0 = temporary target of dot/sel
                                            c1 = tuple name
                                            c2 = tuple field
  To know more detail, see my note
  https://drive.google.com/open?id=16DSzAPf0GzuxYptxkZzdPKJDDP8DxnBz
*/

void Lnast::dot2hier_tuple_chain(const Lnast_nid &psts_nid, Lnast_nid &dot_nid, const Lnast_nid &cond_nid, bool is_else_sts) {
  auto &dot_lrhs_table  =  dot_lrhs_tables[psts_nid];

  auto c0_dot      = get_first_child(dot_nid); //c0 = intermediate target
  auto c1_dot      = get_sibling_next(c0_dot);
  auto c2_dot      = get_sibling_next(c1_dot);

  if (is_lhs(psts_nid, dot_nid)) {
    auto paired_assign_nid = dot_lrhs_table[dot_nid].second;
    auto c0_assign = get_first_child(paired_assign_nid);
    auto c1_assign = get_sibling_next(c0_assign);

    bool c1_assign_is_const = get_data(c1_assign).type.is_const();
    // change node semantic from dot/sel->TG; assign-> phiTA w/ (phi + TA);
    // also add four children for the new phi node, three childre for new TA node
    ref_data(dot_nid)->type           = Lnast_ntype::create_tuple_get();

    ref_data(paired_assign_nid)->type = Lnast_ntype::create_tuple_phi_add();
    ref_data(c0_assign)->type         = Lnast_ntype::create_phi();
    ref_data(c1_assign)->type         = Lnast_ntype::create_tuple_add();

    // rename for redibility
    auto &new_phi_nid = c0_assign;
    auto &new_ta_nid  = c1_assign;

    // handle the new phi
    auto &tg_nid      = dot_nid;
    auto c0_tg        = get_first_child(tg_nid);

    auto c0_phi = add_child(new_phi_nid, Lnast_node::create_ref(add_string(absl::StrCat("___tup_tmp_", tup_internal_cnt))));
    tup_internal_cnt += 1;
    add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_cond(), get_token(cond_nid), get_subs(cond_nid)));
    if (is_else_sts) {
      add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_ref(), get_token(c0_tg), get_subs(c0_tg)));
      if (c1_assign_is_const) {
        add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_const(), get_token(c1_assign), get_subs(c1_assign)));
      } else {
        add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_ref(), get_token(c1_assign), get_subs(c1_assign)));
      }
    } else {
      if (c1_assign_is_const) {
        add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_const(), get_token(c1_assign), get_subs(c1_assign)));
      } else {
        add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_ref(), get_token(c1_assign), get_subs(c1_assign)));
      }
      add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_ref(), get_token(c0_tg), get_subs(c0_tg)));
    }

    ref_data(new_phi_nid)->token = Token();


    // handle the new tuple_add
    add_child(new_ta_nid, Lnast_node(Lnast_ntype::create_ref(), get_token(c1_dot), get_subs(c1_dot)));
    if (get_data(c2_dot).type.is_const()) {
      add_child(new_ta_nid, Lnast_node(Lnast_ntype::create_const(), get_token(c2_dot), get_subs(c2_dot)));
    } else {
      add_child(new_ta_nid, Lnast_node(Lnast_ntype::create_ref(), get_token(c2_dot), get_subs(c2_dot)));
    }
    add_child(new_ta_nid, Lnast_node(Lnast_ntype::create_ref(), get_token(c0_phi), get_subs(c0_phi)));

    ref_data(new_ta_nid)->token = Token();

  } else { // is rhs
    // change node semantic from dot/set->tuple_get
    ref_data(dot_nid)->type = Lnast_ntype::create_tuple_get();
  }
}


bool Lnast::check_tuple_table_parents_chain(const Lnast_nid &psts_nid, std::string_view ref_name) {
  if (get_parent(psts_nid) == get_root()) {
    auto &tuple_var_table = tuple_var_tables[psts_nid];
    return tuple_var_table.find(ref_name)!= tuple_var_table.end();

  } else {
    auto tmp_if_nid = get_parent(psts_nid);
    auto new_psts_nid = get_parent(tmp_if_nid);
    auto &tuple_var_table = tuple_var_tables[new_psts_nid];
    if (tuple_var_table.find(ref_name) != tuple_var_table.end()) {
      return true;
    } else {
      return check_tuple_table_parents_chain(new_psts_nid, ref_name);
    }
  }
}


void Lnast::analyze_dot_lrhs(const Lnast_nid &psts_nid) {
  Dot_lrhs_table  top_dot_lrhs_table;
  dot_lrhs_tables[psts_nid] = top_dot_lrhs_table;
  for (const auto &opr_nid : children(psts_nid)) {
    auto type = get_type(opr_nid);
    if (type.is_func_def()) {
      do_ssa_trans(opr_nid);
    } else if (type.is_if()) {
      analyze_dot_lrhs_if_subtree(opr_nid);
    } else if (type.is_dot() || type.is_select() || type.is_tuple_concat() || type.is_tuple()) {
      analyze_dot_lrhs_handle_a_statement(psts_nid, opr_nid);
    }
  }
}

void Lnast::analyze_dot_lrhs_handle_a_statement(const Lnast_nid &psts_nid, const Lnast_nid &dot_nid) {
  auto type = get_type(dot_nid);
  I(type.is_dot() || type.is_select() || type.is_tuple_concat() || type.is_tuple());
  auto &dot_lrhs_table = dot_lrhs_tables[psts_nid];
  auto c0_dot      = get_first_child(dot_nid); //c0 = intermediate target
  auto c0_dot_name = get_name(c0_dot);
  bool hit         = false;
  auto sib_nid     = dot_nid;
  while (!hit) {
    if (sib_nid == get_last_child(psts_nid))
      return;

    sib_nid = get_sibling_next(sib_nid);

    // hier-tuple case
    if (get_type(sib_nid).is_tuple()) {
      for (auto sib_child : children(sib_nid)) {
        if (sib_child == get_first_child(sib_nid))
          continue;

        if (!get_type(sib_child).is_assign())
          continue;

        auto c0_assign = get_first_child(sib_child);
        auto c1_assign = get_sibling_next(c0_assign);
        if (get_name(c1_assign) == c0_dot_name) {
          hit = true;
          dot_lrhs_table[dot_nid].first  = false;
          dot_lrhs_table[dot_nid].second = Lnast_nid(-1, -1);
        }
      }

      if (hit)
        continue;
    }

    // normal case
    for (auto sib_child : children(sib_nid)) {
      if (sib_child == get_first_child(sib_nid) && get_name(sib_child) == c0_dot_name && !get_type(sib_nid).is_if()) {
        hit = true;
        dot_lrhs_table[dot_nid].first = true; //is lhs
        dot_lrhs_table[dot_nid].second = sib_nid;
        break;
      } else if (get_name(sib_child) == c0_dot_name){
        hit = true;
        dot_lrhs_table[dot_nid].first  = false;
        dot_lrhs_table[dot_nid].second = sib_nid;
        break;
      }
    }
  } //note: practically, the assign/opr_op related to the dot/sel_op should be very close
}


void Lnast::analyze_dot_lrhs_if_subtree(const Lnast_nid &if_nid) {
  for (const auto &itr_nid : children(if_nid)) {
    if (get_type(itr_nid).is_stmts()) {
      Cnt_rtable if_sts_ssa_rhs_cnt_table;
      Dot_lrhs_table  if_sts_dot_lrhs_table;
      dot_lrhs_tables[itr_nid] = if_sts_dot_lrhs_table;

      for (const auto &opr_nid : children(itr_nid)) {
        I(!get_type(opr_nid).is_func_def());
        if (get_type(opr_nid).is_if())
          analyze_dot_lrhs_if_subtree(opr_nid);
        else if (get_type(opr_nid).is_dot() || get_type(opr_nid).is_select())
          analyze_dot_lrhs_handle_a_statement(itr_nid, opr_nid);
      }
    } else if (get_type(itr_nid).is_phi()){
      //FIXME->sh: check with phi
      continue;
    } else { //condition node
      continue;
    }
  }
}


void Lnast::resolve_ssa_lhs_subs(const Lnast_nid &psts_nid) {
  for (const auto &opr_nid : children(psts_nid)) {
    if (get_type(opr_nid).is_func_def()) {
      continue;
    } else if (get_type(opr_nid).is_if()) {
      ssa_lhs_if_subtree(opr_nid);
    } else {
      ssa_lhs_handle_a_statement(psts_nid, opr_nid);
    }
  }
}


void Lnast::resolve_ssa_rhs_subs(const Lnast_nid &psts_nid) {
  Cnt_rtable top_ssa_rhs_cnt_table;
  ssa_rhs_cnt_tables[psts_nid] = top_ssa_rhs_cnt_table;
  for (const auto &opr_nid : children(psts_nid)) {
    if (get_type(opr_nid).is_func_def()) {
      continue;
    } else if (get_type(opr_nid).is_if()) {
      ssa_rhs_if_subtree(opr_nid);
    } else {
      ssa_rhs_handle_a_statement(psts_nid, opr_nid);
    }
  }
}

void Lnast::ssa_rhs_if_subtree(const Lnast_nid &if_nid) {
  for (const auto &itr_nid : children(if_nid)) {
    if (get_type(itr_nid).is_stmts()) {
      Cnt_rtable if_sts_ssa_rhs_cnt_table;
      ssa_rhs_cnt_tables[itr_nid] = if_sts_ssa_rhs_cnt_table;

      for (const auto &opr_nid : children(itr_nid)) {
        I(!get_type(opr_nid).is_func_def());
        if (get_type(opr_nid).is_if())
          ssa_rhs_if_subtree(opr_nid);
        else
          ssa_rhs_handle_a_statement(itr_nid, opr_nid);
      }
    } else if (get_type(itr_nid).is_phi()){
      update_rhs_ssa_cnt_table(get_parent(if_nid), get_first_child(itr_nid));
    } else { //condition node
      continue;
    }
  }
}


void Lnast::ssa_rhs_handle_a_statement(const Lnast_nid &psts_nid, const Lnast_nid &opr_nid) {
  const auto type = get_type(opr_nid);

  if (type.is_dot() || type.is_select())
    I(false);

  if (type.is_tuple()) {
    for (auto itr_opd : children(opr_nid)) {
      if (itr_opd == get_first_child(opr_nid)) continue;
      ssa_rhs_handle_a_statement(psts_nid, itr_opd);
    }
  } else {
    //handle statement rhs of normal operators
    for (auto itr_opd : children(opr_nid)) {
      if (itr_opd == get_first_child(opr_nid)) continue;
      ssa_rhs_handle_a_operand(psts_nid, itr_opd);
    }
  }

  //handle statement lhs
  if (type.is_assign() || type.is_as() || type.is_dp_assign() || type.is_attr_set()) {
    const auto  target_nid  = get_first_child(opr_nid);
    const auto  target_name = get_name(target_nid);

    if (target_name.substr(0,3) == "___") return;

    update_rhs_ssa_cnt_table(psts_nid, target_nid);
  }
}


void Lnast::opr_lhs_merge(const Lnast_nid &psts_nid) {
  for (const auto &opr_nid : children(psts_nid)) {
    if (get_type(opr_nid).is_func_def()) {
      continue;
    } else if (get_type(opr_nid).is_if()) {
      opr_lhs_merge_if_subtree(opr_nid);
    } else if (get_type(opr_nid).is_assign()){
      opr_lhs_merge_handle_a_statement(opr_nid);
    }
  }
}


void Lnast::opr_lhs_merge_if_subtree(const Lnast_nid &if_nid) {
  for (const auto &itr_nid : children(if_nid)) {
    if (get_type(itr_nid).is_stmts()) {
      Cnt_rtable if_sts_ssa_rhs_cnt_table;
      ssa_rhs_cnt_tables[itr_nid] = if_sts_ssa_rhs_cnt_table;

      for (const auto &opr_nid : children(itr_nid)) {
        I(!get_type(opr_nid).is_func_def());
        if (get_type(opr_nid).is_if())
          opr_lhs_merge_if_subtree(opr_nid);
        else if (get_type(opr_nid).is_assign())
          opr_lhs_merge_handle_a_statement(opr_nid);
      }
    }
  }
}


void Lnast::opr_lhs_merge_handle_a_statement(const Lnast_nid &assign_nid) {
  const auto  c0_assign      = get_first_child(assign_nid);
  const auto  c1_assign_name = get_name(get_sibling_next(c0_assign));

  if (c1_assign_name.substr(0,3) == "___") {
    auto opr_nid = get_sibling_prev(assign_nid);
    auto c0_opr = get_first_child(opr_nid);
    I(get_name(c0_opr) == c1_assign_name);
    ref_data(c0_opr)->token = get_data(c0_assign).token;
    ref_data(c0_opr)->type = get_data(c0_assign).type;
    ref_data(c0_opr)->subs = get_data(c0_assign).subs;
    ref_data(assign_nid)->type = Lnast_ntype::create_invalid();
  }
}


//handle cases: A.foo = A[2] or A.foo = A[1] + A[2] + A.bar; where lhs rhs are both the struct elements;
//the ssa should be: A_2.foo = A_1[2] or A_6.foo = A_5[1] + A_5[2] + A_5.bar
bool Lnast::is_special_case_of_dot_rhs(const Lnast_nid &psts_nid, const Lnast_nid &opr_nid) {
  auto &dot_lrhs_table = dot_lrhs_tables[psts_nid];
  I(!is_lhs(psts_nid, opr_nid));

  if (opr_nid == get_first_child(psts_nid))
    return false;

  auto prev_sib_nid = get_sibling_prev(opr_nid);

  if ((get_type(prev_sib_nid).is_dot() || get_type(prev_sib_nid).is_select())) {
    if (!dot_lrhs_table[prev_sib_nid].first) {
      return is_special_case_of_dot_rhs(psts_nid, prev_sib_nid);
    } else if (dot_lrhs_table[prev_sib_nid].first) {
      return true;
    }
  }
  return false;
}

void Lnast::ssa_rhs_handle_a_operand_special(const Lnast_nid &gpsts_nid, const Lnast_nid &opd_nid) {
  //note: immediate struct self assignment: A.foo = A[2], which will leads to consecutive dot and sel,
  //the sel should follow the subscript before the dot increments it.
  auto &ssa_rhs_cnt_table = ssa_rhs_cnt_tables[gpsts_nid];
  auto       opd_name  = get_name(opd_nid);
  const auto opd_type  = get_type(opd_nid);
  Token      ori_token = get_token(opd_nid);

  if (ssa_rhs_cnt_table.find(opd_name) != ssa_rhs_cnt_table.end()) {
    auto  new_subs = ssa_rhs_cnt_table[opd_name] - 1;
    set_data(opd_nid, Lnast_node(opd_type, ori_token, new_subs));
  }
}


void Lnast::ssa_rhs_handle_a_operand(const Lnast_nid &gpsts_nid, const Lnast_nid &opd_nid) {
  auto &ssa_rhs_cnt_table = ssa_rhs_cnt_tables[gpsts_nid];
  auto       opd_name  = get_name(opd_nid);
  const auto opd_type  = get_type(opd_nid);
  Token      ori_token = get_token(opd_nid);

  if (ssa_rhs_cnt_table.find(opd_name) != ssa_rhs_cnt_table.end()) {
    auto  new_subs = ssa_rhs_cnt_table[opd_name];
    set_data(opd_nid, Lnast_node(opd_type, ori_token, new_subs));
  } else {
    int8_t  new_subs = check_rhs_cnt_table_parents_chain(gpsts_nid, opd_nid);
    if (new_subs == -1 && !is_reg(opd_name)) { //if the register opd_subs is -1, it is intentionally to do it to recognize reg qpin
      new_subs = 0; //FIXME->sh: actually, here is a good place to check undefined variable
    }
    ssa_rhs_cnt_table[opd_name] = new_subs;
    set_data(opd_nid, Lnast_node(opd_type, ori_token, new_subs));
  }
}


void Lnast::ssa_lhs_if_subtree(const Lnast_nid &if_nid) {
  for (const auto &itr_nid : children(if_nid)) {
    if (get_type(itr_nid).is_stmts()) {
      Phi_rtable if_sts_phi_resolve_table;
      phi_resolve_tables[itr_nid] = if_sts_phi_resolve_table;

      for (const auto &opr_nid : children(itr_nid)) {
        I(!get_type(opr_nid).is_func_def());
        if (get_type(opr_nid).is_if())
          ssa_lhs_if_subtree(opr_nid);
        else
          ssa_lhs_handle_a_statement(itr_nid, opr_nid);
      }
    } else { //condition node or csts
      continue;
    }
  }
  ssa_handle_phi_nodes(if_nid);
}

void Lnast::ssa_handle_phi_nodes(const Lnast_nid &if_nid) {
  std::vector<Lnast_nid> if_stmts_vec;
  for (const auto &itr : children(if_nid)) {
    if (get_type(itr).is_stmts())
      if_stmts_vec.push_back(itr);
  }

  //2 possible cases: (1)if-elif-elif (2) if-elif-else
  //note: handle reversely to get correct mux priority chain
  for (auto itr = if_stmts_vec.rbegin(); itr != if_stmts_vec.rend(); ++itr) {
    if (itr == if_stmts_vec.rbegin() && has_else_stmts(if_nid)) {
      continue;
    } else if (itr == if_stmts_vec.rbegin() && !has_else_stmts(if_nid)) {
      Phi_rtable &true_table  = phi_resolve_tables[*itr];
      Phi_rtable fake_false_table ; //for the case of if-elif-elif
      Lnast_nid condition_nid = get_sibling_prev(*itr);
      resolve_phi_nodes(condition_nid, true_table, fake_false_table);
    } else if (itr == if_stmts_vec.rbegin()+1 && has_else_stmts(if_nid)) {
      Phi_rtable &true_table  = phi_resolve_tables[*itr];
      Phi_rtable &false_table = phi_resolve_tables[*if_stmts_vec.rbegin()];
      Lnast_nid condition_nid = get_sibling_prev(*itr);
      resolve_phi_nodes(condition_nid, true_table, false_table);
    } else {
      Phi_rtable &true_table  = phi_resolve_tables[*itr];
      Phi_rtable &false_table = new_added_phi_node_tables[get_parent(*itr)];
      Lnast_nid condition_nid = get_sibling_prev(*itr);
      resolve_phi_nodes(condition_nid, true_table, false_table);
    }
  }
}


void Lnast::resolve_phi_nodes(const Lnast_nid &cond_nid, Phi_rtable &true_table, Phi_rtable &false_table) {
  for (auto const&[key, val] : false_table) {
    /* fmt::print("false table content: key->{}, value->{}\n", key, get_token(val).get_text()); */
    if (true_table.find(key) != true_table.end()) {
      add_phi_node(cond_nid, true_table[key], false_table[key]);
      true_table.erase(key);
    } else {
      auto if_nid = get_parent(cond_nid);
      auto psts_nid = get_parent(if_nid);
      auto t_nid = get_complement_nid(key, psts_nid, false);
      add_phi_node(cond_nid, t_nid, false_table[key]);
    }
  }

  std::vector<std::string_view> key_list;
  for (auto const&[key, val] : true_table) {
    if (true_table.empty()) // it might be empty due to the erase from previous for loop
      break;

    if (false_table.find(key) != false_table.end()) {
      add_phi_node(cond_nid, true_table[key], false_table[key]);
      key_list.push_back(key);
    } else {
      auto if_nid = get_parent(cond_nid);
      auto psts_nid = get_parent(if_nid);
      auto f_nid = get_complement_nid(key, psts_nid, true);
      add_phi_node(cond_nid, true_table[key], f_nid);
      key_list.push_back(key);
    }
  }

  for (auto key : key_list) {
    true_table.erase(key);
  }
}


Lnast_nid Lnast::get_complement_nid(std::string_view brother_name, const Lnast_nid &psts_nid, bool false_path) {
  auto if_nid = get_parent(psts_nid);
  Phi_rtable &new_added_phi_node_table = new_added_phi_node_tables[if_nid];
  if(false_path && new_added_phi_node_table.find(brother_name) != new_added_phi_node_table.end()) {
    return new_added_phi_node_table[brother_name];
  }
  else {
    return check_phi_table_parents_chain(brother_name, psts_nid, false);
  }
}


Lnast_nid Lnast::check_phi_table_parents_chain(std::string_view target_name, const Lnast_nid &psts_nid, bool originate_from_csts) {
  auto &parent_table = phi_resolve_tables[psts_nid];

  if(parent_table.find(target_name) != parent_table.end())
    return parent_table[target_name];

  if (get_parent(psts_nid) == get_root() && originate_from_csts) {
    ; // do nothing for csts
  } else if (get_parent(psts_nid) == get_root() && !originate_from_csts){
    if (is_reg(target_name)) {
      return add_child(psts_nid, Lnast_node(Lnast_ntype::create_reg_fwd(),  Token(Token_id_alnum, 0, 0, 0, "register_forwarding")));
    } else {
      return add_child(psts_nid, Lnast_node(Lnast_ntype::create_err_flag(), Token(Token_id_alnum, 0, 0, 0, "err_var_undefined")));
    }
  } else {
    auto tmp_if_nid = get_parent(psts_nid);
    auto new_psts_nid = get_parent(tmp_if_nid);
    return check_phi_table_parents_chain(target_name, new_psts_nid, originate_from_csts);
  }

  I(false); // what return value? not-deterministic result
  return Lnast_nid();
}


Lnast_nid Lnast::add_phi_node(const Lnast_nid &cond_nid, const Lnast_nid &t_nid, const Lnast_nid &f_nid) {

  auto if_nid = get_parent(cond_nid);
  Phi_rtable &new_added_phi_node_table = new_added_phi_node_tables[if_nid];
  auto new_phi_nid = add_child(if_nid, Lnast_node(Lnast_ntype::create_phi(), Token()));
  Lnast_nid target_nid;
  if (get_type(t_nid).is_err_flag() || get_type(f_nid).is_err_flag()) {
    target_nid  = add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_err_flag(), Token(Token_id_alnum, 0, 0, 0, "err_var_undefined"))); //ssa update later
  } else {
    target_nid  = add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_ref(), get_token(t_nid), get_subs(t_nid))); //ssa update later
  }
  update_global_lhs_ssa_cnt_table(target_nid);
  add_child(new_phi_nid, Lnast_node(Lnast_ntype::create_cond(), get_token(cond_nid), get_subs(cond_nid)));
  add_child(new_phi_nid, Lnast_node(get_type(t_nid),  get_token(t_nid), get_subs(t_nid)));
  add_child(new_phi_nid, Lnast_node(get_type(f_nid),  get_token(f_nid), get_subs(f_nid)));
  new_added_phi_node_table[get_name(target_nid)] = target_nid;

  auto psts_nid = get_parent(if_nid);
  update_phi_resolve_table(psts_nid, target_nid);
  return target_nid;
}


bool Lnast::has_else_stmts(const Lnast_nid &if_nid) {
  Lnast_nid last_child = get_last_child(if_nid);
  Lnast_nid second_last_child = get_sibling_prev(last_child);
  return (get_type(last_child).is_stmts() && get_type(second_last_child).is_stmts());
}

void Lnast::ssa_lhs_handle_a_statement(const Lnast_nid &psts_nid, const Lnast_nid &opr_nid) {
  //handle lhs of the statement, handle statement rhs in the 2nd part SSA

  const auto type     = get_type(opr_nid);
  const auto lhs_nid  = get_first_child(opr_nid);

  if (type.is_tuple_add() || type.is_tuple_concat())
    respect_latest_global_lhs_ssa(lhs_nid);


  if (type.is_assign()     || type.is_dp_assign() || type.is_as() || 
      type.is_tuple()      || type.is_attr_set()  || type.is_func_call() ||
      type.is_logical_op() || type.is_unary_op()  || 
      type.is_nary_op()    || type.is_tuple_get()) {
    const auto lhs_name = get_name(lhs_nid);
    if (!lhs_name.empty() && lhs_name.substr(0,3) == "___")
      return;

    update_global_lhs_ssa_cnt_table(lhs_nid);
    update_phi_resolve_table(psts_nid, lhs_nid);
    return;
  }

  //handle rhs of the statement, but only care about rhs register here
  for (const auto &itr : children(opr_nid)) {
    if (itr == get_first_child(opr_nid))
      continue;

    if (is_reg(get_name(itr)))
      reg_ini_global_lhs_ssa_cnt_table(itr);
  }
}

bool Lnast::is_lhs(const Lnast_nid &psts_nid, const Lnast_nid &opr_nid) {
  I(get_type(opr_nid).is_dot() || get_type(opr_nid).is_select());
  auto &dot_lrhs_table = dot_lrhs_tables[psts_nid];
  if (dot_lrhs_table.find(opr_nid)!= dot_lrhs_table.end())
    return dot_lrhs_table[opr_nid].first;

  I(false);
  return false;
}

void Lnast::reg_ini_global_lhs_ssa_cnt_table(const Lnast_nid &rhs_nid) {
  //initialize global reg to zero when appeared in rhs
  const auto  rhs_name = get_name(rhs_nid);
  auto itr = global_ssa_lhs_cnt_table.find(rhs_name);
  if (itr != global_ssa_lhs_cnt_table.end()) {
    return;
  } else {
    global_ssa_lhs_cnt_table[rhs_name] = -1; //FIXME->sh: no effect? check later
  }
}


void Lnast::respect_latest_global_lhs_ssa(const Lnast_nid &lhs_nid) {
  const auto  lhs_name = get_name(lhs_nid);
  auto itr = global_ssa_lhs_cnt_table.find(lhs_name);
  if (itr != global_ssa_lhs_cnt_table.end()) {
    ref_data(lhs_nid)->subs = itr->second;
  } else {
    global_ssa_lhs_cnt_table[lhs_name] = 0;
  }
}

void Lnast::update_global_lhs_ssa_cnt_table(const Lnast_nid &lhs_nid) {
  const auto  lhs_name = get_name(lhs_nid);
  auto itr = global_ssa_lhs_cnt_table.find(lhs_name);
  if (itr != global_ssa_lhs_cnt_table.end()) {
    itr->second += 1;
    ref_data(lhs_nid)->subs = itr->second;
  } else {
    global_ssa_lhs_cnt_table[lhs_name] = 0;
  }
}


//note: the subs of the lhs of the operator has already handled clearly in first round ssa process, just copy into the rhs_ssa_cnt_table fine.
void Lnast::update_rhs_ssa_cnt_table(const Lnast_nid &psts_nid, const Lnast_nid &target_key) {
  auto &ssa_rhs_cnt_table = ssa_rhs_cnt_tables[psts_nid];
  const auto target_name = get_name(target_key);
  ssa_rhs_cnt_table[target_name] = ref_data(target_key)->subs;
}

int8_t Lnast::check_rhs_cnt_table_parents_chain(const Lnast_nid &psts_nid, const Lnast_nid &target_key) {

  auto &ssa_rhs_cnt_table = ssa_rhs_cnt_tables[psts_nid];
  const auto  target_name = get_name(target_key);
  auto itr = ssa_rhs_cnt_table.find(target_name);

  if (itr != ssa_rhs_cnt_table.end()) {
    return ssa_rhs_cnt_table[target_name];
  } else if (get_parent(psts_nid) == get_root()) {
    return -1;
  } else if (get_type(get_parent(psts_nid)).is_func_def()) {
    return -1;
  } else {
    auto tmp_if_nid = get_parent(psts_nid);
    auto new_psts_nid = get_parent(tmp_if_nid);
    return check_rhs_cnt_table_parents_chain(new_psts_nid, target_key);
  }
}

void Lnast::update_phi_resolve_table(const Lnast_nid &psts_nid, const Lnast_nid &target_nid) {
  auto       &phi_resolve_table = phi_resolve_tables[psts_nid];
  const auto  target_name       = get_name(target_nid);
  phi_resolve_table[target_name] = target_nid; //for a variable string, always update to latest Lnast_nid
}

bool Lnast::is_in_bw_table(const std::string_view name) {
  return from_lgraph_bw_table.contains((std::string)name);
}

uint32_t Lnast::get_bitwidth(const std::string_view name) {
  I(is_in_bw_table(name));
  return from_lgraph_bw_table[(std::string)name];
}

void Lnast::set_bitwidth(const std::string_view name, const uint32_t bitwidth) {
  I(bitwidth > 0);
  from_lgraph_bw_table[(std::string)name] = bitwidth;
}

void Lnast::dump() const {
  for (const auto &it : depth_preorder(get_root())) {
    const auto &node = get_data(it);
    std::string indent{"  "};
    for (int i = 0; i < it.level; ++i) indent += "  ";

    fmt::print("{} {} {:>20} : {}\n", it.level, indent, node.type.to_s(), node.token.get_text());
  }
}

/*
Note I: if not handle ssa cnt on lhs and rhs separately, there will be a race condition in the
      if-subtree between child-True and child-False. For example, in the following source code:

      A = 5
      A = A + 4
      if (condition)
        A = A + 1
        A = A + 2
      else
        A = A + 3
      %out = A

      So the new ssa transformation has two main part. The first and original algorithm
      focus on the lhs ssa cnt, phi-node resolving, and phi-node insertion. The lhs ssa
      cnt only need a global count table and the cnt of lhs on every expression will be
      handled correctly. The second algorithm focus on rhs assignment by using tree rhs
      cnt tables.

      rhs assignment in 2nd algorithm:
      - check current scope
      - if exists
          use the local count from local table
        else
          if not in parents chain
            compile error
          else
            copy from parents to local tables and
            use it as cnt
      lhs assignment in 2nd algorithm:
      - just copy the subs from the lnast nodes into the local table
        as the lhs subs has been handled in 1st algorithm.
*/
