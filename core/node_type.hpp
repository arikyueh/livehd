//  this file is distributed under the bsd 3-clause license. see license for details.
#pragma once

#include "lgraphbase.hpp"
#include "mmap_bimap.hpp"
#include "mmap_map.hpp"
#include "mmap_vector.hpp"
#include "cell.hpp"
#include "sub_node.hpp"
#include "node.hpp"

using Node_down_map = mmap_lib::map<Node::Compact_class, Lg_type_id>;

class LGraph_Node_Type : virtual public LGraph_Base {
protected:
  using Node_value_map = mmap_lib::map<Node::Compact_class, Lconst::Container>;
  using Node_lut_map   = mmap_lib::map<Node::Compact_class, Lconst::Container>;

  Node_value_map const_map;  // bimap to avoid unnecessary constant replication

  Node_down_map subid_map;
  Node_lut_map  lut_map;

  void clear();

  void             set_type(Index_ID nid, const Ntype_op op);
  Ntype_op          get_type_op(Index_ID nid) const {
    I(node_internal[nid].is_master_root());
    return node_internal[nid].get_type();
  }

  bool is_type_const(Index_ID nid) const;

  void       set_type_sub(Index_ID nid, Lg_type_id subgraphid);
  Lg_type_id get_type_sub(Index_ID nid) const;

  const Sub_node &get_type_sub_node(Index_ID nid) const;
  const Sub_node &get_type_sub_node(std::string_view sub_name) const;
  Sub_node *      ref_type_sub_node(Index_ID nid);
  Sub_node *      ref_type_sub_node(std::string_view sub_name);

  void   set_type_lut(Index_ID nid, const Lconst &lutid);
  Lconst get_type_lut(Index_ID nid) const;

  void set_type_const(Index_ID nid, const Lconst &value);
  void set_type_const(Index_ID nid, std::string_view value);
  void set_type_const(Index_ID nid, int64_t value);

  // No const because Lconst created
  Lconst get_type_const(Index_ID nid) const;

public:
  LGraph_Node_Type() = delete;
  explicit LGraph_Node_Type(std::string_view path, std::string_view name, Lg_type_id lgid) noexcept;

  const Node_down_map &get_down_nodes_map() const { return subid_map; };
};
