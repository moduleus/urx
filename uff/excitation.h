#pragma once

#include <uff/uff.h>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace uff {

/**
 * @brief The UFF class describing an unique ultrasound excitation
 * 
 * The excitation caracterized the ultrasonic wave at a channel level
 */
struct Excitation {
  auto operator<=>(const Excitation& other) const = default;

  /// Desciption of the pulse shape (e.g., sinusoidal, square wave, chirp), including necessary parameters
  std::string pulseShape = "";

  /// Transmit frequency of the pulser [Hz]
  double transmit_frequency = UFF_NAN;

  /// Sampling frequency of the excitation waveform [Hz]
  double sampling_frequency = UFF_NAN;

  /// Sampled excitation waveform [normalized units]
  /// The waveform values are spaced by a delay equal to 1/transmit_frequency
  std::vector<double> waveform;
};

}  // namespace uff
