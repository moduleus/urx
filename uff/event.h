#pragma once

#include <uff/receive_setup.h>
#include <uff/transmit_setup.h>
#include <uff/trigger_base.h>
#include <uff/uff.h>
#include <iosfwd>

namespace uff {

/**
 * @brief UFF class to describe an unique ultrasound event, composed by a single transmit and receive setup
 */
class Event : public TriggerBase {
 public:
  // CTOR & DTOR
  Event() = delete;
  Event(const TransmitSetup& transmit_setup, const ReceiveSetup& receive_setup)
      : _transmit_setup(transmit_setup), _receive_setup(receive_setup) {}
  Event(const Event&) = default;
  Event(Event&&) = default;
  ~Event() override = default;

  // Operators
  Event& operator=(const Event& other) noexcept = default;
  Event& operator=(Event&& other) noexcept = default;
  inline bool operator==(const Event& other) const {
    return (TriggerBase::operator==(other) && (_transmit_setup == other._transmit_setup) &&
            (_receive_setup == other._receive_setup));
  }
  inline bool operator!=(const Event& other) const { return !(*this == other); }

  // Accessors
  inline TransmitSetup& transmitSetup() { return _transmit_setup; }
  inline void setTransmitSetup(const TransmitSetup& transmit_setup) {
    _transmit_setup = transmit_setup;
  }

  inline ReceiveSetup& receiveSetup() { return _receive_setup; }
  inline void setReceiveSetup(const ReceiveSetup& receive_setup) { _receive_setup = receive_setup; }

  // Members
 private:
  // Description of the transmit event (probe/channels, waves, excitations, etc.). If more than one probe is used in reception, this is a list of setups.
  TransmitSetup _transmit_setup;

  // Description of sampled channel data (probe/channels, sampling, TGC, etc.). If more than one probe is used in reception, this is a list of setups.
  ReceiveSetup _receive_setup;
};

}  // namespace uff
