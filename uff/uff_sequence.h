/*!
 * Copyright Moduleus
 * \file uff_channel_data.h
 * \brief
 */

#ifndef UFF_SEQUENCE_H
#define UFF_SEQUENCE_H

// UFF
#include "uff_object.h"
#include "uff_timed_event.h"

// System
#include <string>

namespace uff
{

    /**
     * @brief UFF class that contains all the information needed to store and later process channel data.
     */
    class Sequence : public uff::Object
    {
        UFF_TYPE_MACRO(Sequence, uff::Object);

    public:
        Sequence() {}
        Sequence(const double timeOffset, const std::vector<TimedEvent> timedEvents)
            : m_timeOffset(timeOffset), m_timedEvents(timedEvents)
        {}

        void printSelf(std::ostream& os, std::string indent) const override;

        bool operator ==(const Sequence& other) const;
        inline bool operator !=(const Sequence& other) const { return !(*this == other); }

        // ******************** Getters / Setters **********************************************
    public:
        // Time offset
        double timeOffset() const { return m_timeOffset; }
        void setTimeOffset(double timeOffset) { m_timeOffset = timeOffset; }

        // timed Events
        const std::vector<TimedEvent>& timedEvents() const { return m_timedEvents; }
        void setTimedEvents(std::vector<TimedEvent> timedEvents) { m_timedEvents = timedEvents; }

        // ******************** Attributes **********************************************
    private:
        // Time offset
        double m_timeOffset = 0;

        // timedEvents
        std::vector<TimedEvent> m_timedEvents;
    };

} // namespace uff

#endif // UFF_SEQUENCE_H
