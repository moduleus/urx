#pragma once

#include <vector>

#include <uff/time_offset_base.h>
#include <uff/timed_event.h>
#include <uff/trigger_base.h>

namespace uff {

/**
 * @brief UFF class that contains all the information needed to store and later process channel data.
 */
class Sequence : public TimeOffsetBase, TriggerBase {
 public:
  // CTOR & DTOR
  Sequence() = delete;
  explicit Sequence(std::vector<TimedEvent> timed_events, MetadataType time_offset = 0.,
                    const std::optional<TriggerIn>& trigger_in = std::nullopt,
                    const std::optional<TriggerOut>& trigger_out = std::nullopt)
      : TimeOffsetBase(time_offset),
        TriggerBase(trigger_in, trigger_out),
        _timed_events(std::move(timed_events)) {}
  Sequence(const Sequence&) = default;
  Sequence(Sequence&&) = default;
  ~Sequence() override = default;

  // Operators
  Sequence& operator=(const Sequence& other) noexcept = default;
  Sequence& operator=(Sequence&& other) noexcept = default;
  bool operator==(const Sequence& other) const {
    return TimeOffsetBase::operator==(other) && TriggerBase::operator==(other) &&
           _timed_events == other._timed_events;
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
};

}  // namespace uff