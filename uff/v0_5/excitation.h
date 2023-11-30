/*!
 * Copyright Moduleus
 * \file uff/excitation.h
 * \brief
 */

#ifndef UFF_EXCITATION_H
#define UFF_EXCITATION_H

#include <iosfwd>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "uff/v0_5/object.h"
#include "uff/v0_5/uff.h"

namespace uff::v0_5 {

/**
 * @brief The UFF Excitation class
 */
class Excitation : public uff::Object {
  UFF_TYPE_MACRO(Excitation, uff::Object);

 public:
  Excitation() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  const std::optional<std::string>& pulseShape() const { return m_pulseShape; }
  void setPulseShape(const std::optional<std::string>& pulseShape) { m_pulseShape = pulseShape; }

  const std::vector<MetadataType>& waveform() const { return m_waveform; }
  void setWaveform(const std::vector<MetadataType>& waveform) { m_waveform = waveform; }

  std::optional<MetadataType> samplingFrequency() const { return m_samplingFrequency; }
  void setSamplingFrequency(std::optional<MetadataType> samplingFrequency) {
    m_samplingFrequency = samplingFrequency;
  }

  std::optional<MetadataType> transmitFrequency() const { return m_transmitFrequency; }
  void setTransmitFrequency(std::optional<MetadataType> transmitFrequency) {
    m_transmitFrequency = transmitFrequency;
  }

  bool operator==(const Excitation& other) const {
    return ((m_pulseShape == other.m_pulseShape) &&
            (m_transmitFrequency == other.m_transmitFrequency) &&
            (m_waveform == other.m_waveform) && (m_samplingFrequency == other.m_samplingFrequency));
  }

  inline bool operator!=(const Excitation& other) const { return !(*this == other); }

 private:
  // String describing the pulse shape (e.g., sinusoidal, square wave, chirp), including necessary parameters
  std::optional<std::string> m_pulseShape = std::nullopt;

  // Transmit frequency [Hz]
  std::optional<MetadataType> m_transmitFrequency = std::nullopt;

  // Sampled excitation waveform [normalized units]
  std::vector<MetadataType> m_waveform;

  // Sampling frequency of the excitation waveform [Hz]
  std::optional<MetadataType> m_samplingFrequency = std::nullopt;
};

}  // namespace uff::v0_5

#endif  // UFF_EXCITATION_H
