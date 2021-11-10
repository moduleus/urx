/*!
 * Copyright Moduleus
 * \file uff_super_group.h
 * \brief
 */

#ifndef UFF_SUPER_GROUP_H
#define UFF_SUPER_GROUP_H

// UFF
#include "uff_object.h"
#include "uff_igroup.h"
#include "uff_group.h"

namespace uff
{

    /**
     * @brief UFF class that contains all the information needed to store and later process channel data.
     */
    class SuperGroup : public IGroup
    {
        UFF_TYPE_MACRO(SuperGroup, uff::Object);

    public:
        SuperGroup() {}
        SuperGroup(const std::weak_ptr<Group> initialGroup)
            : m_initialGroup(initialGroup)
        {}

        void printSelf(std::ostream& os, std::string indent) const override;

        bool operator ==(const SuperGroup& other) const;
        inline bool operator !=(const SuperGroup& other) const { return !(*this == other); }

    private:
        // Initial Group
        std::weak_ptr<Group> m_initialGroup;
    };

} // namespace uff

#endif // UFF_SUPER_GROUP_H
