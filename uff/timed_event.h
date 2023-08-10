#pragma once

#include <iosfwd>
#include <memory>
#include <utility>

#include <uff/receive_setup.h>
#include <uff/time_offset_base.h>
#include <uff/transmit_setup.h>
#include <uff/trigger_base.h>
#include <uff/uff.h>

namespace uff {

/**
 * @brief UFF class to describe an unique ultrasound event, composed by a single transmit and receive setup
 */
class TimedEvent : public TimeOffsetBase, TriggerBase {
 public:
  // CTOR & DTOR
  TimedEvent() = delete;
  explicit TimedEvent(std::weak_ptr<TransmitSetup> transmit_setup,
                      std::weak_ptr<ReceiveSetup> receive_setup, MetadataType time_offset = 0.)
      : TimeOffsetBase(time_offset),
        _transmit_setup(std::move(transmit_setup)),
        _receive_setup(std::move(receive_setup)) {}
  TimedEvent(const TimedEvent&) = default;
  TimedEvent(TimedEvent&&) = default;
  ~TimedEvent() override = default;

  // Operators
  TimedEvent& operator=(const TimedEvent& other) noexcept = default;
  TimedEvent& operator=(TimedEvent&& other) noexcept = default;
  bool operator==(const TimedEvent& other) const {
    return TimeOffsetBase::operator==(other) && TriggerBase::operator==(other) &&
           (_transmit_setup.expired() == other._transmit_setup.expired()) &&
           (_transmit_setup.expired() ||
            *(_transmit_setup.lock()) == *(other._transmit_setup.lock())) &&
           (_receive_setup.expired() == other._receive_setup.expired()) &&
           (_receive_setup.expired() || *(_receive_setup.lock()) == *(other._receive_setup.lock()));
  }
  inline bool operator!=(const TimedEvent& other) const { return !(*this == other); }

  // Accessors
  inline std::weak_ptr<TransmitSetup> transmitSetup() { return _transmit_setup; }
  inline void setTransmitSetup(std::weak_ptr<TransmitSetup> transmit_setup) {
    _transmit_setup = std::move(transmit_setup);
  }

  inline std::weak_ptr<ReceiveSetup> receiveSetup() { return _receive_setup; }
  inline void setReceiveSetup(std::weak_ptr<ReceiveSetup> receive_setup) {
    _receive_setup = std::move(receive_setup);
  }

  // Members
 private:
  // Description of the transmit event (probe/channels, waves, excitations, etc.). If more than one probe is used in reception, this is a list of setups.
  std::weak_ptr<TransmitSetup> _transmit_setup;

  // Description of sampled channel data (probe/channels, sampling, TGC, etc.). If more than one probe is used in reception, this is a list of setups.
  std::weak_ptr<ReceiveSetup> _receive_setup;
};

}  // namespace uff
