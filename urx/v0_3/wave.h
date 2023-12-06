#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/aperture.h>
#include <urx/v0_3/excitation.h>
#include <urx/v0_3/position.h>
#include <urx/v0_3/transform.h>
#include <urx/v0_3/wave_type.h>

namespace urx::v0_3 {

struct Wave {
  Transform origin;
  WaveType wave_type;
  std::shared_ptr<Aperture> aperture;
  Position time_zero_reference_point;
  double time_zero;
  std::vector<std::vector<size_t>> channel_mapping;
  std::vector<Excitation> channel_excitations;
  std::vector<double> channel_delays;
};

}  // namespace urx::v0_3
