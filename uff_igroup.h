/*!
 * Copyright Moduleus
 * \file uff_channel_data.h
 * \brief
 */

#ifndef UFF_IGROUP_H
#define UFF_IGROUP_H

// UFF
#include "uff_object.h"

// System
#include <string>

namespace uff
{

    /**
     * @brief UFF class that contains all the information needed to store and later process channel data.
     */
    class IGroup : public uff::Object
    {
        UFF_TYPE_MACRO(IGroup, uff::Object);

    public:
        IGroup() {}
        IGroup(const std::string description, const double timeOffset, const uint32_t repetitionCount) 
            : m_description(description), m_timeOffset(timeOffset), m_repetitionCount(repetitionCount)
        {}

        void printSelf(std::ostream& os, std::string indent) const override;

        bool operator ==(const IGroup& other) const;
        inline bool operator !=(const IGroup& other) const { return !(*this == other); }

    private:
        // string describing the group
        std::string m_description = "";

        // Time offset
        double m_timeOffset = 0;

        // Group repetition
        uint32_t m_repetitionCount = 1;
    };

} // namespace uff

#endif // UFF_IGROUP_H
