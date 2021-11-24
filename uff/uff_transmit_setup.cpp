/*!
 * Copyright Moduleus
 * \file uff_transmit_setup.cpp
 * \brief
 */

#include "uff_transmit_setup.h"
 
namespace uff 
{

void TransmitSetup::printSelf(std::ostream& os, std::string indent) const
{
    superclass::printSelf(os, indent);
    os << indent << "Time Offset: " << this->timeOffset() << std::endl;

    // Probe
    os << indent << "Probe: ";
    if (auto spt = this->probe().lock()) { (*spt).printSelf(os, indent + UFF_STD_INDENT); }
    else {  os << "<Probe pointer is expired>" << std::endl; }

    // Wave
    os << indent << "Wave: ";
    if (auto spt = this->wave().lock()) { (*spt).printSelf(os, indent + UFF_STD_INDENT); }
    else { os << "<Wave pointer is expired>" << std::endl; }
}

} // namespace uff