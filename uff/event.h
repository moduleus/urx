#pragma once

#include <uff/receive_setup.h>
#include <uff/transmit_setup.h>
#include <uff/uff.h>
#include <iosfwd>

namespace uff {

/**
 * @brief UFF class to describe an unique ultrasound event, composed by a single transmit and receive setup
 */
class Event {
  // CTOR & DTOR
 public:
  Event() = default;
  Event(const Event&) = default;
  Event(Event&&) = default;
  ~Event() = default;

  // Operators
 public:
  Event& operator=(const Event& other) noexcept = default;
  Event& operator=(Event&& other) noexcept = default;
  inline bool operator==(const Event& other) const {
    return ((_transmit_setup == other._transmit_setup) && (_receive_setup == other._receive_setup));
  }
  inline bool operator!=(const Event& other) const { return !(*this == other); }

  // Accessors
 public:
  inline uff::TransmitSetup& transmitSetup() { return _transmit_setup; }
  inline void setTransmitSetup(const uff::TransmitSetup& transmit_setup) {
    _transmit_setup = transmit_setup;
  }

  inline uff::ReceiveSetup& receiveSetup() { return _receive_setup; }
  inline void setReceiveSetup(const uff::ReceiveSetup& receive_setup) {
    _receive_setup = receive_setup;
  }

  // Members
 private:
  // Description of the transmit event (probe/channels, waves, excitations, etc.). If more than one probe is used in reception, this is a list of setups.
  uff::TransmitSetup _transmit_setup;

  // Description of sampled channel data (probe/channels, sampling, TGC, etc.). If more than one probe is used in reception, this is a list of setups.
  uff::ReceiveSetup _receive_setup;
};

}  // namespace uff
