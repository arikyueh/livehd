//  This file is distributed under the BSD 3-Clause License. See LICENSE for details.

#include "prp_lnast.hpp"

#include "lbench.hpp"

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s file\n", argv[0]);
    exit(1);
  }
  Prp_lnast converter;

  Lbench b("inou.PYROPE_prp_lnast_parse");
  converter.parse_file(argv[1]);
  b.end();

  Lbench b2("inou.PYROPE_prp_lnast_convert");
  auto   lnast = converter.prp_ast_to_lnast("test");
  b2.end();

  fmt::print("AST to LNAST output:\n\n");

  std::string rule_name;
#if 1
  for (const auto &it : lnast->depth_preorder(lnast->get_root())) {
    auto        node = lnast->get_data(it);
    std::string indent{"  "};
    for (int i = 0; i < it.level; ++i) indent += "  ";

    /* fmt::print("{} {} {:>20} : {}\n", it.level, indent, lnast->lnast_type_to_string(node.type), node.token.text); */
  }
#endif
  lnast->dump();

  return 0;
}
