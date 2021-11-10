/*!
 * Copyright Moduleus
 * \file uff_igroup.cpp
 * \brief
 */

#include "uff_igroup.h"

namespace uff
{
    
    void IGroup::printSelf(std::ostream& os, std::string indent) const
    {
        os << indent << "Todo IGroup::printSelf()" << std::endl;
    }

    bool IGroup::operator==(const IGroup& other) const
    {
        return m_description == other.m_description &&
            m_timeOffset == other.m_timeOffset &&
            m_repetitionCount == other.m_repetitionCount;
    }

} // namespace uff
