#pragma once

#include <memory>
#include <vector>

#include <uff/probe.h>
#include <uff/transmit_wave.h>
#include <uff/uff.h>

namespace uff {

class TransmitSetup {
  // CTOR & DTOR
 public:
  TransmitSetup() = default;
  TransmitSetup(const TransmitSetup&) = default;
  TransmitSetup(TransmitSetup&&) = default;
  ~TransmitSetup() = default;

  // Operators
 public:
  TransmitSetup& operator=(const TransmitSetup& other) noexcept = default;
  TransmitSetup& operator=(TransmitSetup&& other) noexcept = default;
  bool operator==(const TransmitSetup& other) const {
    return ((_probe.expired() == other._probe.expired()) &&
            (_probe.expired() || (*(_probe.lock()) == *(other._probe.lock()))) &&
            (_transmit_wave == other._transmit_wave) &&
            (_channel_mapping == other._channel_mapping));
  }
  inline bool operator!=(const TransmitSetup& other) const { return !(*this == other); }

  // Accessors
 public:
  inline std::weak_ptr<uff::Probe> probe() const { return _probe; }
  inline void setProbe(std::weak_ptr<uff::Probe> probe) { _probe = std::move(probe); }

  inline uff::TransmitWave getTransmitWave() const { return _transmit_wave; }
  inline uff::TransmitWave& transmitWave() { return _transmit_wave; }
  inline void setTransmitWave(const uff::TransmitWave& transmitWave) {
    _transmit_wave = transmitWave;
  }

  inline std::vector<int> channelMapping() const { return _channel_mapping; }
  inline void setChannelMapping(std::vector<int> channelMapping) {
    _channel_mapping = std::move(channelMapping);
  }

  // Members
 private:
  // Reference to the probe use in transmission
  std::weak_ptr<uff::Probe> _probe;

  // List of transmit waves used in this event with their respective time offset and weight
  uff::TransmitWave _transmit_wave;

  // Map of transmit channels to transducer elements
  std::vector<int> _channel_mapping;
};

}  // namespace uff
