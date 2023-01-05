/*!
 * Copyright Moduleus
 * \file uff_super_group.h
 * \brief
 */

#ifndef UFF_GROUP_DATA_H
#define UFF_GROUP_DATA_H

// UFF
#include "uff_object.h"
#include "uff_group.h"

namespace uff
{

    /**
     * @brief UFF class that contains all the information needed to store and later process channel data.
     */
    class GroupData : public uff::Object
    {
        UFF_TYPE_MACRO(GroupData, uff::Object);

    public:
        GroupData() {}
        GroupData(const std::weak_ptr<Group> group, const std::vector<int16_t>& data)
            : m_group(group), m_data(data)
        {}

        // Group
        const std::weak_ptr<Group>& group() const { return m_group; }
        void setGroup(std::weak_ptr<Group> group) { m_group = group; }

        // Data
        std::vector<int16_t>& data() { return m_data; }
        int16_t* pData() { return m_data.data(); }
        void setData(const std::vector<int16_t>& data) { m_data = data; }

        void printSelf(std::ostream& os, std::string indent) const override;

        bool operator ==(const GroupData& other) const;
        inline bool operator !=(const GroupData& other) const { return !(*this == other); }

    private:
        // Group
        std::weak_ptr<Group> m_group;

        // Data organized as data[iFrame][iEvent][iChannel][iSample].
        std::vector<int16_t> m_data;
    };

} // namespace uff

#endif // UFF_SUPER_GROUP_H
