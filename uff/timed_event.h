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
struct TimedEvent {
  bool operator==(const TimedEvent& other) const {
    return (trigger_in == other.trigger_in) && (trigger_out == other.trigger_out) &&
           (time_offset == other.time_offset) &&
           (transmit_setup.expired() == other.transmit_setup.expired()) &&
           (transmit_setup.expired() ||
            *(transmit_setup.lock()) == *(other.transmit_setup.lock())) &&
           (receive_setup.expired() == other.receive_setup.expired()) &&
           (receive_setup.expired() || *(receive_setup.lock()) == *(other.receive_setup.lock()));
  }
  inline bool operator!=(const TimedEvent& other) const { return !(*this == other); }

  // Description of the transmit event (probe/channels, waves, excitations, etc.). If more than one probe is used in reception, this is a list of setups.
  std::weak_ptr<TransmitSetup> transmit_setup;

  // Description of sampled channel data (probe/channels, sampling, TGC, etc.). If more than one probe is used in reception, this is a list of setups.
  std::weak_ptr<ReceiveSetup> receive_setup;

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff
