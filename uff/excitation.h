#pragma once

#include <uff/uff.h>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace uff {

struct Excitation {
  auto operator<=>(const Excitation& other) const = default;

  // String describing the pulse shape (e.g., sinusoidal, square wave, chirp), including necessary parameters
  std::optional<std::string> pulseShape = std::nullopt;

  // Transmit frequency [Hz]
  std::optional<double> transmit_frequency = std::nullopt;

  // Sampling frequency of the excitation waveform [Hz]
  double sampling_frequency;

  // Sampled excitation waveform [normalized units]
  std::vector<double> waveform;
};

}  // namespace uff
