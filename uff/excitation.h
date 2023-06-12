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
  // CTOR & DTOR
 public:
  Excitation() = default;
  Excitation(const Excitation&) = default;
  Excitation(Excitation&&) = default;
  ~Excitation() = default;

  // Operators
 public:
  Excitation& operator=(const Excitation& other) noexcept = default;
  Excitation& operator=(Excitation&& other) noexcept = default;
  inline bool operator==(const Excitation& other) const {
    return ((_pulseShape == other._pulseShape) &&
            (_transmitFrequency == other._transmitFrequency) && (_waveform == other._waveform) &&
            (_sampling_frequency == other._sampling_frequency));
  }
  inline bool operator!=(const Excitation& other) const { return !(*this == other); }

  // Accessors
 public:
  inline const std::optional<std::string>& pulseShape() const { return _pulseShape; }
  inline void setPulseShape(const std::optional<std::string>& pulseShape) {
    _pulseShape = pulseShape;
  }

  inline const std::vector<MetadataType>& waveform() const { return _waveform; }
  inline void setWaveform(const std::vector<MetadataType>& waveform) { _waveform = waveform; }

  inline std::optional<MetadataType> samplingFrequency() const { return _sampling_frequency; }
  inline void setSamplingFrequency(std::optional<MetadataType> samplingFrequency) {
    _sampling_frequency = samplingFrequency;
  }

  inline std::optional<MetadataType> transmitFrequency() const { return _transmitFrequency; }
  inline void setTransmitFrequency(std::optional<MetadataType> transmitFrequency) {
    _transmitFrequency = transmitFrequency;
  }

  // Members
 private:
  // String describing the pulse shape (e.g., sinusoidal, square wave, chirp), including necessary parameters
  std::optional<std::string> _pulseShape = std::nullopt;

  // Transmit frequency [Hz]
  std::optional<MetadataType> _transmitFrequency = std::nullopt;

  // Sampled excitation waveform [normalized units]
  std::vector<MetadataType> _waveform;

  // Sampling frequency of the excitation waveform [Hz]
  std::optional<MetadataType> _sampling_frequency = std::nullopt;
};

}  // namespace uff
