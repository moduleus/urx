/*!
 * Copyright Moduleus
 * \file uff_group_data.cpp
 * \brief
 */

#include "uff_group_data.h"

namespace uff
{
    
    void GroupData::printSelf(std::ostream& os, std::string indent) const
    {
        os << indent << "Todo SuperGroup::printSelf()" << std::endl;
    }

    bool GroupData::operator==(const GroupData& other) const
    {
        return m_group.lock() == other.m_group.lock();
    }

} // namespace uff
