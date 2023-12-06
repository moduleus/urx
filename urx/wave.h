#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/excitation.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/urx.h>
#include <urx/vector.h>

namespace urx {

struct Wave {
  enum class WaveType {
    CONVERGING_WAVE = 0,
    DIVERGING_WAVE = 1,
    PLANE_WAVE = 2,
    CYLINDRICAL_WAVE = 3,
    UNDEFINED = -1
  };

  bool operator==(const Wave& other) const = default;

  WaveType type = WaveType::UNDEFINED;

  DoubleNan time_zero;

  Vector3D<double> time_zero_reference_point;

  std::vector<std::vector<uint32_t>> channel_mapping;

  std::vector<std::weak_ptr<Excitation>> channel_excitations;

  std::vector<double> channel_delays;

  std::vector<double> parameters;
};

}  // namespace urx
