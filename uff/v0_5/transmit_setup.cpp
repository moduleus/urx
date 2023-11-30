/*!
 * Copyright Moduleus
 * \file uff/transmit_setup.cpp
 * \brief
 */

#include "uff/v0_5/transmit_setup.h"

#include <ostream>

namespace uff::v0_5 {

void TransmitSetup::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Channel mapping size: " << this->channelMapping().size() << std::endl
     << indent << "Probe: ";
  if (auto spt = this->probe().lock()) {  // Has to be copied into a shared_ptr before usage
    //(*spt).printSelf(os, indent + UFF_STD_INDENT);
    os << &(*spt) << std::endl;
  } else {
    os << "<Probe pointer is expired>" << std::endl;
  }
  os << indent << "TransmitWave: ";
  m_transmitWave.printSelf(os, indent + UFF_STD_INDENT);
}

}  // namespace uff::v0_5