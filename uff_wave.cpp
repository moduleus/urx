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

    os << indent << "Excitation: ";
    this->m_excitation.printSelf(os, indent + UFF_STD_INDENT);
}

} // namespace uff