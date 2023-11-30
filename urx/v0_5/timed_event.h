/*!
 * Copyright Moduleus
 * \file urx/timed_event.h
 * \brief
 */

#ifndef URX_TIMED_EVENT_H
#define URX_TIMED_EVENT_H

#include <iosfwd>
#include <memory>
#include <string>
#include <utility>

#include <urx/v0_5/event.h>
#include <urx/v0_5/object.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

/**
 * @brief The URX TimedEvent class
 */
class TimedEvent : public Object {
  URX_TYPE_MACRO(TimedEvent, Object);

 public:
  TimedEvent() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::weak_ptr<Event> evenement() const { return m_event; }
  void setEvent(std::weak_ptr<Event> event) { m_event = std::move(event); }

  MetadataType timeOffset() const { return m_timeOffset; }
  void setTimeOffset(MetadataType timeOffset) { m_timeOffset = timeOffset; }

  bool operator==(const TimedEvent& other) const {
    return ((m_timeOffset == other.m_timeOffset) &&
            (m_event.expired() == other.m_event.expired()) &&
            (m_event.expired() || (*(m_event.lock()) == *(other.m_event.lock()))));
  }

  inline bool operator!=(const TimedEvent& other) const { return !(*this == other); }

 private:
  //    Reference to one of the unique transmit/receive events used in the sequence.
  std::weak_ptr<Event> m_event;

  // time offset relative to start of the sequence repetition (frame) [s]
  MetadataType m_timeOffset = 0.f;
};

}  // namespace urx::v0_5

#endif  // URX_TIMED_EVENT_H