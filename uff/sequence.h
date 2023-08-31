#pragma once

#include <vector>

#include <uff/timed_event.h>
#include <uff/trigger_in.h>
#include <uff/trigger_out.h>

namespace uff {

/**
 * @brief UFF class that contains all the information needed to store and later process channel data.
 */
struct Sequence {
  bool operator==(const Sequence& other) const {
    return (timed_events == other.timed_events) && (trigger_in == other.trigger_in) &&
           (trigger_out == other.trigger_out) && (time_offset == other.time_offset);
  }
  inline bool operator!=(const Sequence& other) const { return !(*this == other); }

  std::vector<TimedEvent> timed_events;

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff