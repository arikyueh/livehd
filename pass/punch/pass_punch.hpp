//  This file is distributed under the BSD 3-Clause License. See LICENSE for details.
#pragma once

#include <string_view>

#include "pass.hpp"
#include "hierarchy.hpp"
#include "node_pin.hpp"

class Pass_punch : public Pass {
protected:
  Hierarchy_index src_hierarchy;  // FIXME: _index does not include information about the pin or internal node
  Hierarchy_index dst_hierarchy;

  static void work(Eprp_var &var);

public:
  Pass_punch(const Eprp_var &var);
  static void setup();

  void punch(LGraph *top, std::string_view src, std::string_view dst);

  void add_output(LGraph *g, std::string_view wname, std::string_view output);
  void add_output(LGraph *g, Node_pin dpin, std::string output);

  bool add_input(LGraph *g, std::string_view wire, std::string_view input);
  bool add_dest_instance(LGraph *g, std::string_view type, std::string_view instance, std::string_view wire);
};
