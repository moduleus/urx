#pragma once

#include <string>
#include <vector>

namespace urx::v0_3 {

struct Excitation {
  std::string pulse_shape;
  double transmit_frequency;
  double sampling_frequency;
  std::vector<double> waveform;
};

}  // namespace urx::v0_3
