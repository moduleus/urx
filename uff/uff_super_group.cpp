/*!
 * Copyright Moduleus
 * \file uff_super_group.cpp
 * \brief
 */

#include "uff_super_group.h"

namespace uff
{
    
    void SuperGroup::printSelf(std::ostream& os, std::string indent) const
    {
        os << indent << "Todo SuperGroup::printSelf()" << std::endl;
    }

    bool SuperGroup::operator==(const SuperGroup& other) const
    {
        return m_initialGroup.lock() == other.m_initialGroup.lock();
    }

} // namespace uff
