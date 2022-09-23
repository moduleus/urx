/*!
 * Copyright Moduleus
 * \file uff/receive_setup.h
 * \brief
 */

#ifndef UFF_RECEIVE_SETUP_H
#define UFF_RECEIVE_SETUP_H

// UFF
#include "uff/object.h"
#include "uff/probe.h"

// System
#include <iostream>
#include <memory>

namespace uff {

/**
 * @brief The UFF ReceiveSetup class
 */
class ReceiveSetup : public uff::Object {
  UFF_TYPE_MACRO(ReceiveSetup, uff::Object);

 public:
  enum class SAMPLING_TYPE { DIRECT_RF = 0, QUADRATURE_4X_F0 = 1, QUADRATURE_2X_F0 = 2, IQ = 3 };

  ReceiveSetup() {}

  void printSelf(std::ostream& os, std::string indent) const override;

  const std::weak_ptr<uff::Probe> probe() const { return m_probe; }
  void setProbe(std::weak_ptr<uff::Probe> probe) { m_probe = probe; }

  FloatingType timeOffset() const { return m_timeOffset; }
  void setTimeOffset(FloatingType timeOffset) { m_timeOffset = timeOffset; }

  std::optional<FloatingType> samplingFrequency() const { return m_samplingFrequency; }
  void setSamplingFrequency(std::optional<FloatingType> samplingFrequency) {
    m_samplingFrequency = samplingFrequency;
  }

  SAMPLING_TYPE samplingType() const { return m_samplingType; }
  void setSamplingType(SAMPLING_TYPE samplingType) { m_samplingType = samplingType; }

  std::vector<int> channelMapping() const { return m_channelMapping; }
  void setChannelMapping(const std::vector<int>& channelMapping) {
    m_channelMapping = channelMapping;
  }

  std::vector<FloatingType> tgcProfile() const { return m_tgcProfile; }
  void setTgcProfile(const std::vector<FloatingType>& tgcProfile) { m_tgcProfile = tgcProfile; }

  std::optional<FloatingType> tgcSamplingFrequency() const { return m_tgcSamplingFrequency; }
  void setTgcSamplingFrequency(std::optional<FloatingType> tgcSamplingFrequency) {
    m_tgcSamplingFrequency = tgcSamplingFrequency;
  }

  std::optional<FloatingType> modulationFrequency() const { return m_modulationFrequency; }
  void setModulationFrequency(std::optional<FloatingType> modulationFrequency) {
    m_modulationFrequency = modulationFrequency;
  }

  bool operator==(const ReceiveSetup& other) const {
    return ((m_probe.expired() == other.m_probe.expired()) &&
            (m_probe.expired() || (*(m_probe.lock()) == *(other.m_probe.lock()))) &&
            (m_timeOffset == other.m_timeOffset) &&
            (m_samplingFrequency == other.m_samplingFrequency) &&
            (m_samplingType == other.m_samplingType) &&
            (m_channelMapping == other.m_channelMapping) && (m_tgcProfile == other.m_tgcProfile) &&
            (m_tgcSamplingFrequency == other.m_tgcSamplingFrequency) &&
            (m_modulationFrequency == other.m_modulationFrequency));
  }

  inline bool operator!=(const ReceiveSetup& other) const { return !(*this == other); }

 private:
  // Probes used for this receive setup
  std::weak_ptr<uff::Probe> m_probe;

  // Time before the first samples [s]
  FloatingType m_timeOffset = 0.;

  // Sampling frequency [Hz]
  std::optional<FloatingType> m_samplingFrequency = std::nullopt;

  // Type of sampling:
  SAMPLING_TYPE m_samplingType = SAMPLING_TYPE::DIRECT_RF;

  // Maps the RF line with the corresponding element
  // channel_element_index = m_channelMapping[rf_index]
  std::vector<int> m_channelMapping;

  // (Optional) Analog TGC profile sampled at tgc_sampling_frequency [dB]
  std::vector<FloatingType> m_tgcProfile = {0.f};

  //     (Optional) Sampling frequency of the TGC profile [Hz]
  std::optional<FloatingType> m_tgcSamplingFrequency = std::nullopt;

  //     (Optional) Modulation frequency used in case of IQ-data [Hz]
  std::optional<FloatingType> m_modulationFrequency = std::nullopt;
};

}  // namespace uff

#endif  // UFF_RECEIVE_SETUP_H
