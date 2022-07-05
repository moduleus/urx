/*!
 * Copyright Moduleus
 * \file uff_transmit_setup.cpp
 * \brief
 */

#include "uff_transmit_setup.h"

namespace uff {

void TransmitSetup::printSelf(std::ostream& os, std::string indent) const {
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

}  // namespace uff