/*!
 * Copyright Moduleus
 * \file urx/transmit_setup.cpp
 * \brief
 */

#include <ios>
#include <ostream>

#include <urx/v0_5/object.h>
#include <urx/v0_5/transmit_setup.h>
#include <urx/v0_5/transmit_wave.h>

namespace urx::v0_5 {

void TransmitSetup::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Channel mapping size: " << this->channelMapping().size() << '\n'
     << indent << "Probe: ";
  if (auto spt = this->probe().lock()) {  // Has to be copied into a shared_ptr before usage
    //(*spt).printSelf(os, indent + URX_STD_INDENT);
    os << &(*spt) << '\n';
  } else {
    os << "<Probe pointer is expired>" << '\n';
  }
  os << indent << "TransmitWave: ";
  m_transmitWave.printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_5