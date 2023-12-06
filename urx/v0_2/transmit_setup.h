// Copyright Moduleus

#ifndef URX_TRANSMIT_SETUP_H
#define URX_TRANSMIT_SETUP_H

#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/v0_2/object.h>
#include <urx/v0_2/probe.h>
#include <urx/v0_2/transmit_wave.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief The URX TransmitSetup class
 */
class TransmitSetup : public Object {
  URX_TYPE_MACRO(TransmitSetup, Object);

 public:
  TransmitSetup() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::weak_ptr<Probe> probe() const { return m_probe; }
  void setProbe(std::weak_ptr<Probe> probe) { m_probe = std::move(probe); }

  TransmitWave getTransmitWave() const { return m_transmitWave; }
  TransmitWave& transmitWave() { return m_transmitWave; }
  void setTransmitWave(const TransmitWave& transmitWave) { m_transmitWave = transmitWave; }

  const std::vector<int>& channelMapping() const { return m_channelMapping; }
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
  std::weak_ptr<Probe> m_probe;

  // List of transmit waves used in this event with their respective time offset and weight
  TransmitWave m_transmitWave;

  // Map of transmit channels to transducer elements
  std::vector<int> m_channelMapping;
};

}  // namespace urx::v0_2

#endif  // URX_TRANSMIT_SETUP_H