/*!
 * Copyright Moduleus
 * \file uff/transmit_setup.h
 * \brief
 */

#ifndef UFF_TRANSMIT_SETUP_H
#define UFF_TRANSMIT_SETUP_H

#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "uff/v0_5/object.h"
#include "uff/v0_5/probe.h"
#include "uff/v0_5/transmit_wave.h"
#include "uff/v0_5/uff.h"

namespace uff::v0_5 {

/**
 * @brief The UFF TransmitSetup class
 */
class TransmitSetup : public uff::Object {
  UFF_TYPE_MACRO(TransmitSetup, uff::Object);

 public:
  TransmitSetup() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::weak_ptr<uff::Probe> probe() const { return m_probe; }
  void setProbe(std::weak_ptr<uff::Probe> probe) { m_probe = std::move(probe); }

  uff::TransmitWave getTransmitWave() const { return m_transmitWave; }
  uff::TransmitWave& transmitWave() { return m_transmitWave; }
  void setTransmitWave(const uff::TransmitWave& transmitWave) { m_transmitWave = transmitWave; }

  std::vector<int> channelMapping() const { return m_channelMapping; }
  void setChannelMapping(std::vector<int> channelMapping) {
    m_channelMapping = std::move(channelMapping);
  }

  bool operator==(const TransmitSetup& other) const {
    return ((m_probe.expired() == other.m_probe.expired()) &&
            (m_probe.expired() || (*(m_probe.lock()) == *(other.m_probe.lock()))) &&
            (m_transmitWave == other.m_transmitWave) &&
            (m_channelMapping == other.m_channelMapping));
  }

  inline bool operator!=(const TransmitSetup& other) const { return !(*this == other); }

 private:
  // Reference to the probe use in transmission
  std::weak_ptr<uff::Probe> m_probe;

  // List of transmit waves used in this event with their respective time offset and weight
  uff::TransmitWave m_transmitWave;

  // Map of transmit channels to transducer elements
  std::vector<int> m_channelMapping;
};

}  // namespace uff::v0_5

#endif  // UFF_TRANSMIT_SETUP_H