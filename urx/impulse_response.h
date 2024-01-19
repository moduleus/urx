#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/detail/double_nan.h>
#include <urx/urx.h>

namespace urx {

struct ImpulseResponse {
  bool operator==(const ImpulseResponse& other) const {
    return sampling_frequency == other.sampling_frequency && time_offset == other.time_offset &&
           units == other.units && data == other.data;
  }

  bool operator!=(const ImpulseResponse& other) const { return !operator==(other); }

  DoubleNan sampling_frequency;
  DoubleNan time_offset{0};

  std::string units;

  std::vector<double> data;
};

}  // namespace urx
