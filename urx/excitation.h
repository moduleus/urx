#pragma once

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <urx/detail/double_nan.h>
#include <urx/urx.h>

namespace urx {

struct Excitation {
  bool operator==(const Excitation& other) const = default;

  std::string pulse_shape;

  DoubleNan transmit_frequency;
  DoubleNan sampling_frequency;

  std::vector<double> waveform;
};

}  // namespace urx
