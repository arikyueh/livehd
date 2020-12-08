//  This file is distributed under the BSD 3-Clause License. See LICENSE for details.

#include "inou_pyrope.hpp"

#include "annotate.hpp"
#include "lbench.hpp"
#include "lgedgeiter.hpp"
#include "lgraph.hpp"
#include "prp_lnast.hpp"

void setup_inou_pyrope() { Inou_pyrope::setup(); }

void Inou_pyrope::setup() {
  Eprp_method m1("inou.pyrope", "Parse the input file and convert to an LNAST", &Inou_pyrope::parse_to_lnast);
  m1.add_label_required("files", "pyrope files to process (comma separated)");

  register_pass(m1);
}

Inou_pyrope::Inou_pyrope(const Eprp_var &var) : Pass("inou.pyrope", var) {}

void Inou_pyrope::parse_to_lnast(Eprp_var &var) {
  Lbench      b("inou.PYROPE_parse_to_lnast");
  Inou_pyrope p(var);

  for (auto f : absl::StrSplit(p.files, ',')) {
    Prp_lnast converter;
    converter.parse_file(f);

    std::string name{f};
    auto        found_path = name.find_last_of('/');
    if (found_path != std::string::npos)
      name = name.substr(found_path + 1);

    auto found_dot = name.find_last_of('.');
    if (found_dot != std::string::npos)
      name = name.substr(0, found_dot);
    auto lnast = converter.prp_ast_to_lnast(name);
    var.add(std::move(lnast));
  }
}
