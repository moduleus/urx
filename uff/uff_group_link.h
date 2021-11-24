/*!
 * Copyright Moduleus
 * \file uff_group_link.h
 * \brief
 */

#ifndef UFF_GROUP_LINK_H
#define UFF_GROUP_LINK_H

// UFF
#include "uff_object.h"
#include "uff_igroup.h"

// System
#include <string>

namespace uff
{

    /**
     * @brief UFF class that contains all the information needed to store and later process channel data.
     */
    class GroupLink : public uff::Object
    {
        UFF_TYPE_MACRO(GroupLink, uff::Object);

    public:
        GroupLink() {}
        GroupLink(const std::weak_ptr<IGroup> source, const std::weak_ptr<IGroup> destination)
            : m_source(source), m_destination(destination)
        {}

        const std::weak_ptr<IGroup>& source() const { return m_source; }
        void setSource(std::weak_ptr<IGroup> source) { m_source = source; }

        const std::weak_ptr<IGroup>& destination() const { return m_destination; }
        void setDestination(std::weak_ptr<IGroup> destination) { m_destination = destination; }

        void printSelf(std::ostream& os, std::string indent) const override;

        bool operator ==(const GroupLink& other) const;
        inline bool operator !=(const GroupLink& other) const { return !(*this == other); }

    private:
        // Source
        std::weak_ptr<IGroup> m_source;
        std::weak_ptr<IGroup> m_destination;
    };

} // namespace uff

#endif // UFF_IGROUP_H
