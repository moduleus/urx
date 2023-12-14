#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/detail/double_nan.h>
#include <urx/urx.h>

namespace urx {

/**
 * @brief The URX class describing the temporal impulse response
 */
struct ImpulseResponse {
  bool operator==(const ImpulseResponse& other) const = default;

  DoubleNan sampling_frequency;
  DoubleNan time_offset{0};

  std::string units;

  std::vector<double> data;
};

}  // namespace urx
