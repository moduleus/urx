#pragma once

#include <uff/uff.h>

#include <string>
#include <vector>

namespace uff {

/**
 * @brief The UFF ImpulseResponse class specifies a temporal impulse response
 */
struct ImpulseResponse {
  auto operator<=>(const ImpulseResponse& other) const = default;

  // Sampling frequency in Hz
  double sampling_frequency = UFF_NAN;

  // Collection of samples containing the impulse response
  std::vector<double> data;

  // (Optional) Name of the units of the impulse response
  std::string units = "";

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff
