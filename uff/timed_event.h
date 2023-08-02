#pragma once

#include <memory>
#include <utility>

#include <uff/event.h>
#include <uff/time_offset_base.h>
#include <uff/trigger_base.h>
#include <uff/uff.h>

namespace uff {

class TimedEvent : public TimeOffsetBase {
 public:
  // CTOR & DTOR
  TimedEvent() = delete;
  TimedEvent(const std::weak_ptr<Event>& event, MetadataType time_offset = 0.)
      : TimeOffsetBase(time_offset), _event(event) {}
  TimedEvent(const TimedEvent&) = default;
  TimedEvent(TimedEvent&&) = default;
  ~TimedEvent() override = default;

  // Operators
  TimedEvent& operator=(const TimedEvent& other) noexcept = default;
  TimedEvent& operator=(TimedEvent&& other) noexcept = default;
  bool operator==(const TimedEvent& other) const {
    return (TimeOffsetBase::operator==(other) && (_event.expired() == other._event.expired()) &&
            (_event.expired() || (*(_event.lock()) == *(other._event.lock()))));
  }
  inline bool operator!=(const TimedEvent& other) const { return !(*this == other); }

  // Accessors
  inline std::weak_ptr<Event> evenement() const { return _event; }
  inline void setEvent(std::weak_ptr<Event> event) { _event = std::move(event); }

  // Members
 private:
  //    Reference to one of the unique transmit/receive events used in the sequence.
  std::weak_ptr<Event> _event;
};

}  // namespace uff
