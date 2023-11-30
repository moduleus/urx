/*!
 * Copyright Moduleus
 * \file urx/wave.cpp
 * \brief
 */

#include <urx/v0_5/wave.h>

#include <ostream>
#include <utility>

namespace urx::v0_5 {

void Wave::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);

  os << indent << "Origin: ";
  this->m_origin.printSelf(os, indent + URX_STD_INDENT);

  os << indent << "WaveType: " << static_cast<int>(this->m_waveType) << std::endl;

  os << indent << "Aperture: ";
  this->m_aperture.printSelf(os, indent + URX_STD_INDENT);

  os << indent << "Excitation: ";
  this->m_excitation.printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_5