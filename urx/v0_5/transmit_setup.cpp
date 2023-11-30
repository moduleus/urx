/*!
 * Copyright Moduleus
 * \file urx/transmit_setup.cpp
 * \brief
 */

#include <ostream>

#include <urx/v0_5/transmit_setup.h>

namespace urx::v0_5 {

void TransmitSetup::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Channel mapping size: " << this->channelMapping().size() << std::endl
     << indent << "Probe: ";
  if (auto spt = this->probe().lock()) {  // Has to be copied into a shared_ptr before usage
    //(*spt).printSelf(os, indent + URX_STD_INDENT);
    os << &(*spt) << std::endl;
  } else {
    os << "<Probe pointer is expired>" << std::endl;
  }
  os << indent << "TransmitWave: ";
  m_transmitWave.printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_5