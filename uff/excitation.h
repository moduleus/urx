#pragma once

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <uff/detail/double_nan.h>
#include <uff/uff.h>

namespace uff {

/**
 * @brief The UFF class describing an unique ultrasound excitation
 * 
 * The excitation caracterized the ultrasonic wave at a channel level
 */
struct Excitation {
  bool operator==(const Excitation& other) const = default;

  /// Desciption of the pulse shape (e.g., sinusoidal, square wave, chirp), including necessary parameters
  std::string pulse_shape;

  /// Transmit frequency of the pulser [Hz]
  DoubleNan transmit_frequency;

  /// Sampling frequency of the excitation waveform [Hz]
  DoubleNan sampling_frequency;

  /// Sampled excitation waveform [normalized units]
  /// The waveform values are spaced by a delay equal to 1/transmit_frequency
  std::vector<double> waveform;
};

}  // namespace uff
