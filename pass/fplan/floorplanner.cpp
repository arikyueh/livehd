#include "floorplanner.hpp"

#include <functional>

#include "lgedgeiter.hpp"

unsigned int Lhd_floorplanner::get_area(LGraph* lg) {
  // use the number of nodes as an approximation of area
  unsigned int num_nodes = 0;
  for (auto node : lg->fast(true)) {
    (void)node;
    num_nodes++;
  }

  return num_nodes;
}

void Lhd_floorplanner::create_floorplan(const std::string_view filename) {
  bool success = root_layout->layout(AspectRatio, 1);
  if (!success) {
    throw std::runtime_error("unable to lay out floorplan!");
  } else {
    ostream& fos = outputHotSpotHeader(filename.data());
    root_layout->outputHotSpotLayout(fos);
    outputHotSpotFooter(fos);
  }
}

void Lhd_floorplanner::analyze_floorplan() {
  if (!(root_layout)) {
    throw std::runtime_error("no floorplan generated!");
  }
  
  
}