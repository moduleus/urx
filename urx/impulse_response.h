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

  /// Sampling frequency [Hz]
  DoubleNan sampling_frequency;

  /// Collection of samples containing the impulse response
  /// The impulse response data values are spaced by a delay equal to 1/sampling_frequency
  std::vector<double> data;

  /// Name of the units of the impulse response
  std::string units;

  /// Time offset of the impulse response
  DoubleNan time_offset{0};
};

}  // namespace urx
