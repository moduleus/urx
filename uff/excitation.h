#pragma once

#include <uff/uff.h>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace uff {

class Excitation {
 public:
  // CTOR & DTOR
  Excitation() = delete;
  Excitation(MetadataType sampling_frequency) : _sampling_frequency(sampling_frequency) {}
  Excitation(const Excitation&) = default;
  Excitation(Excitation&&) = default;
  ~Excitation() = default;

  // Operators
  Excitation& operator=(const Excitation& other) noexcept = default;
  Excitation& operator=(Excitation&& other) noexcept = default;
  inline bool operator==(const Excitation& other) const {
    return ((_pulseShape == other._pulseShape) &&
            (_transmit_frequency == other._transmit_frequency) && (_waveform == other._waveform) &&
            (_sampling_frequency == other._sampling_frequency));
  }
  inline bool operator!=(const Excitation& other) const { return !(*this == other); }

  // Accessors
  inline const std::optional<std::string>& pulseShape() const { return _pulseShape; }
  inline void setPulseShape(const std::optional<std::string>& pulseShape) {
    _pulseShape = pulseShape;
  }

  inline const std::vector<MetadataType>& waveform() const { return _waveform; }
  inline void setWaveform(const std::vector<MetadataType>& waveform) { _waveform = waveform; }

  inline MetadataType samplingFrequency() const { return _sampling_frequency; }
  inline void setSamplingFrequency(MetadataType sampling_frequency) {
    _sampling_frequency = sampling_frequency;
  }

  inline std::optional<MetadataType> transmitFrequency() const { return _transmit_frequency; }
  inline void setTransmitFrequency(std::optional<MetadataType> transmit_frequency) {
    _transmit_frequency = transmit_frequency;
  }

  // Members
 private:
  // String describing the pulse shape (e.g., sinusoidal, square wave, chirp), including necessary parameters
  std::optional<std::string> _pulseShape = std::nullopt;

  // Transmit frequency [Hz]
  std::optional<MetadataType> _transmit_frequency = std::nullopt;

  // Sampling frequency of the excitation waveform [Hz]
  MetadataType _sampling_frequency;

  // Sampled excitation waveform [normalized units]
  std::vector<MetadataType> _waveform;
};

}  // namespace uff
