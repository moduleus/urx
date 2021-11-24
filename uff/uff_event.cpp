/*!
 * Copyright Moduleus
 * \file uff_event.cpp
 * \brief
 */

#include "uff_event.h"
 
namespace uff 
{

void Event::printSelf(std::ostream& os, std::string indent) const
{
    superclass::printSelf(os, indent);
    
    os << indent << "TransmitSetup: ";
    this->m_transmitSetup.printSelf(os, indent + UFF_STD_INDENT);
    
    os << indent << "ReceiveSetup: ";
    this->m_receiveSetup.printSelf(os, indent + UFF_STD_INDENT);
}

} // namespace uff