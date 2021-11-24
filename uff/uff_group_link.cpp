/*!
 * Copyright Moduleus
 * \file uff_group_link.cpp
 * \brief
 */

#include "uff_group_link.h"

namespace uff
{
    
    void GroupLink::printSelf(std::ostream& os, std::string indent) const
    {
        os << indent << "Todo GroupLink::printSelf()" << std::endl;
    }

    bool GroupLink::operator==(const GroupLink& other) const
    {
        return m_source.lock() == other.m_source.lock() &&
            m_destination.lock() == other.m_destination.lock();
    }

} // namespace uff
