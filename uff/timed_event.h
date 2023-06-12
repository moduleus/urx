#pragma once

#include <memory>
#include <utility>

#include <uff/event.h>
#include <uff/uff.h>

namespace uff {

class TimedEvent {
  // CTOR & DTOR
 public:
  TimedEvent() = default;
  TimedEvent(const TimedEvent&) = default;
  TimedEvent(TimedEvent&&) = default;
  ~TimedEvent() = default;

  // Operators
 public:
  TimedEvent& operator=(const TimedEvent& other) noexcept = default;
  TimedEvent& operator=(TimedEvent&& other) noexcept = default;
  bool operator==(const TimedEvent& other) const {
    return ((_time_offset == other._time_offset) && (_event.expired() == other._event.expired()) &&
            (_event.expired() || (*(_event.lock()) == *(other._event.lock()))));
  }
  inline bool operator!=(const TimedEvent& other) const { return !(*this == other); }

  // Accessors
 public:
  inline std::weak_ptr<uff::Event> evenement() const { return _event; }
  inline void setEvent(std::weak_ptr<uff::Event> event) { _event = std::move(event); }

  inline MetadataType timeOffset() const { return _time_offset; }
  inline void setTimeOffset(MetadataType timeOffset) { _time_offset = timeOffset; }

  // Members
 private:
  //    Reference to one of the unique transmit/receive events used in the sequence.
  std::weak_ptr<uff::Event> _event;

  // time offset relative to start of the sequence repetition (frame) [s]
  MetadataType _time_offset = 0.f;
};

}  // namespace uff
