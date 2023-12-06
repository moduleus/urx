#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/probe.h>

namespace urx::v0_3 {

struct LinearArray : public Probe {
  uint32_t number_elements;
  double pitch;
  double element_width;
  double element_height;
};

}  // namespace urx::v0_3
