/*!
 * Copyright Moduleus
 * \file uff_wave.cpp
 * \brief
 */

#include "uff_wave.h"
 
namespace uff 
{

void Wave::printSelf(std::ostream& os, std::string indent) const
{
    superclass::printSelf(os, indent);

    os << indent << "Origin: ";
    this->m_origin.printSelf(os, indent + UFF_STD_INDENT);

    os << indent << "WaveType: " << (int)this->m_waveType << std::endl;

    os << indent << "Aperture: ";
    this->m_aperture.printSelf(os, indent + UFF_STD_INDENT);

    os << indent << "Channel Mapping size: " << this->m_channelMapping.size() << std::endl;

    os << indent << "Excitation: ";
    if (auto spt = this->m_excitation.lock()) { (*spt).printSelf(os, indent + UFF_STD_INDENT); }
    else { os << "<excitation pointer expired>" << std::endl; }
}

} // namespace uff