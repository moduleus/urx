#pragma once

#include <iosfwd>
#include <memory>
#include <utility>

#include <uff/receive_setup.h>
#include <uff/transmit_setup.h>
#include <uff/trigger_in.h>
#include <uff/trigger_out.h>
#include <uff/uff.h>

namespace uff {

/**
 * @brief UFF class to describe an unique ultrasound event, composed by a single transmit and receive setup
 */
class TimedEvent {
 public:
  // CTOR & DTOR
  TimedEvent() = delete;
  explicit TimedEvent(std::weak_ptr<TransmitSetup> transmit_setup,
                      std::weak_ptr<ReceiveSetup> receive_setup, double time_offset = 0.,
                      const std::optional<TriggerIn>& trigger_in = std::nullopt,
                      const std::optional<TriggerOut>& trigger_out = std::nullopt)
      : _transmit_setup(std::move(transmit_setup)),
        _receive_setup(std::move(receive_setup)),
        _trigger_in(trigger_in),
        _trigger_out(trigger_out),
        _time_offset(time_offset) {}
  TimedEvent(const TimedEvent&) = default;
  TimedEvent(TimedEvent&&) = default;
  ~TimedEvent() = default;

  // Operators
  TimedEvent& operator=(const TimedEvent& other) noexcept = default;
  TimedEvent& operator=(TimedEvent&& other) noexcept = default;
  bool operator==(const TimedEvent& other) const {
    return (_trigger_in == other._trigger_in) && (_trigger_out == other._trigger_out) &&
           (_time_offset == other._time_offset) &&
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

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> _trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> _trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
