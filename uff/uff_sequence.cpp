/*!
 * Copyright Moduleus
 * \file uff_sequence.cpp
 * \brief
 */

#include "uff_sequence.h"

namespace uff
{
    
    void Sequence::printSelf(std::ostream& os, std::string indent) const
    {
        os << indent << "Todo uff_sequence::printSelf()" << std::endl;
    }

    bool Sequence::operator==(const Sequence& other) const
    {
        return m_timeOffset == other.m_timeOffset &&
            m_timedEvents == other.m_timedEvents;
    }

} // namespace uff
