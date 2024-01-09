#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/probe.h>

namespace urx::v0_3 {

struct RcaArray : public Probe {
  uint32_t number_elements_x;
  uint32_t number_elements_y;
  double pitch_x;
  double pitch_y;
  double element_width_x;
  double element_width_y;
  double element_height_x;
  double element_height_y;
};

}  // namespace urx::v0_3
