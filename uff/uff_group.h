/*!
 * Copyright Moduleus
 * \file uff_channel_data.h
 * \brief
 */

#ifndef UFF_GROUP_H
#define UFF_GROUP_H

// UFF
#include "uff_igroup.h"
#include "uff_sequence.h"

// System
#include <string>

namespace uff
{

    /**
     * @brief UFF class that contains all the information needed to store and later process channel data.
     */
    class Group : public IGroup
    {
        UFF_TYPE_MACRO(Group, uff::Object);

    public:
        Group() {}
        Group(const double repetitionRate, const Sequence sequence)
            : m_repetitionRate(repetitionRate), m_sequence(sequence)
        {}

        void printSelf(std::ostream& os, std::string indent) const override;

        bool operator ==(const Group& other) const;
        inline bool operator !=(const Group& other) const { return !(*this == other); }

        // ************************** Convenient Methods ******************************************
    public:
        uint32_t numberOfFrames()  const { return repetitionCount(); }
        uint32_t numberOfEvents()  const { return (uint32_t)m_sequence.timedEvents().size(); }
        uint32_t numberOfChannels()const { return m_sequence.timedEvents().size() > 0 ? (uint32_t)m_sequence.timedEvents()[0].evenement().lock()->receiveSetup().channelMapping().size() : 0; }
        uint32_t numberOfSamples() const { return m_sequence.timedEvents().size() > 0 ? m_sequence.timedEvents()[0].evenement().lock()->receiveSetup().numberOfSamples() : 0; }

        // ************************** Getters / Setters ******************************************
    public:
        // Repetition Rate
        double repetitionRate() const { return m_repetitionRate; }
        void setRepetitionRate(double repetitionRate) { m_repetitionRate = repetitionRate; }

        // Sequence
        const Sequence& sequence() const { return m_sequence; }
        void setSequence(const Sequence& sequence) { m_sequence = sequence; }

        // ************************** Attributes ******************************************
    private:
        // Group repetition
        double m_repetitionRate = 0;

        // Sequence
        Sequence m_sequence;
    };

} // namespace uff

#endif // UFF_GROUP_H
