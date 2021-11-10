/*!
 * Copyright Moduleus
 * \file uff_group.cpp
 * \brief
 */

#include "uff_group.h"

namespace uff
{
    
    void Group::printSelf(std::ostream& os, std::string indent) const
    {
        os << indent << "Todo Group::printSelf()" << std::endl;
    }

    bool Group::operator==(const Group& other) const
    {
        return m_repetitionRate == other.m_repetitionRate &&
            m_sequence == other.m_sequence;
    }

} // namespace uff
