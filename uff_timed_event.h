/*!
 * Copyright Moduleus
 * \file uff_timed_event.h
 * \brief
 */

#ifndef UFF_TIMED_EVENT_H
#define UFF_TIMED_EVENT_H

// UFF
#include "uff_event.h"
#include "uff_object.h"

namespace uff {

/**
 * @brief The UFF TimedEvent class
 */
class TimedEvent : public uff::Object {
  UFF_TYPE_MACRO(TimedEvent, uff::Object);

 public:
  TimedEvent() {}

  void printSelf(std::ostream& os, std::string indent) const override;

  std::weak_ptr<uff::Event> evenement() const { return m_event; }
  void setEvent(std::weak_ptr<uff::Event> event) { m_event = event; }

  FloatingType timeOffset() const { return m_timeOffset; }
  void setTimeOffset(FloatingType timeOffset) { m_timeOffset = timeOffset; }

  bool operator==(const TimedEvent& other) const {
    return (
        (m_timeOffset == other.m_timeOffset) &&
        (m_event.expired() == other.m_event.expired()) &&
        (m_event.expired() || (*(m_event.lock()) == *(other.m_event.lock()))));
  }

  inline bool operator!=(const TimedEvent& other) const {
    return !(*this == other);
  }

 private:
  //    Reference to one of the unique transmit/receive events used in the sequence.
  std::weak_ptr<uff::Event> m_event;

  // time offset relative to start of the sequence repetition (frame) [s]
  FloatingType m_timeOffset = 0.f;
};

}  // namespace uff

#endif  // UFF_TIMED_EVENT_H