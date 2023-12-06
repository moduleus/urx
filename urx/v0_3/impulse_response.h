#pragma once

#include <memory>
#include <string>
#include <vector>

namespace urx::v0_3 {

struct ImpulseResponse {
  double time_offset;
  double sampling_frequency;
  std::vector<double> data;
  std::string units;
};

}  // namespace urx::v0_3
