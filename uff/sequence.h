#pragma once

#include <vector>

#include <uff/timed_event.h>
#include <uff/trigger_in.h>
#include <uff/trigger_out.h>

namespace uff {

/**
 * @brief UFF class that contains all the information needed to store and later process channel data.
 */
class Sequence {
 public:
  // CTOR & DTOR
  Sequence() = delete;
  explicit Sequence(std::vector<TimedEvent> timed_events, double time_offset = 0.,
                    const std::optional<TriggerIn>& trigger_in = std::nullopt,
                    const std::optional<TriggerOut>& trigger_out = std::nullopt)
      : _timed_events(std::move(timed_events)),
        _trigger_in(trigger_in),
        _trigger_out(trigger_out),
        _time_offset(time_offset) {}
  Sequence(const Sequence&) = default;
  Sequence(Sequence&&) = default;
  ~Sequence() = default;

  // Operators
  Sequence& operator=(const Sequence& other) noexcept = default;
  Sequence& operator=(Sequence&& other) noexcept = default;
  bool operator==(const Sequence& other) const {
    return (_timed_events == other._timed_events) && (_trigger_in == other._trigger_in) &&
           (_trigger_out == other._trigger_out) && (_time_offset == other._time_offset);
  }
  inline bool operator!=(const Sequence& other) const { return !(*this == other); }

  // Accessors
  inline const std::vector<TimedEvent>& timedEvents() const { return _timed_events; }
  inline void setTimedEvents(const std::vector<TimedEvent>& timed_events) {
    _timed_events = timed_events;
  }

  // Members
 private:
  std::vector<TimedEvent> _timed_events;

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> _trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> _trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff