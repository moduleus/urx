#pragma once

#include <uff/uff.h>

#include <string>
#include <vector>

namespace uff {

/**
 * @brief The UFF class describing the temporal impulse response
 */
struct ImpulseResponse {
  inline bool operator==(const ImpulseResponse& other) const {
    return (is_nan_or_equal(sampling_frequency, other.sampling_frequency) && (data == other.data) &&
            (units == other.units) && is_nan_or_equal(time_offset, other.time_offset));
  }
  inline bool operator!=(const ImpulseResponse& other) const { return !(*this == other); }

  /// Sampling frequency [Hz]
  double sampling_frequency = UFF_NAN;

  /// Collection of samples containing the impulse response
  /// The impulse response data values are spaced by a delay equal to 1/sampling_frequency
  std::vector<double> data;

  /// (Optional) Name of the units of the impulse response
  std::string units = "";

  /// Time offset of the impulse response
  double time_offset = UFF_NAN;
};

}  // namespace uff
