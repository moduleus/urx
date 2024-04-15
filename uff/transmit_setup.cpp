/*!
 * Copyright Moduleus
 * \file uff/transmit_setup.cpp
 * \brief
 */

#include "uff/transmit_setup.h"

#include <ostream>
#include <string>

namespace uff {

void TransmitSetup::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Channel mapping size: " << this->channelMapping().size() << '\n'
     << indent << "Probe: ";
  if (auto spt = this->probe().lock()) {  // Has to be copied into a shared_ptr before usage
    //(*spt).printSelf(os, indent + UFF_STD_INDENT);
    os << &(*spt) << '\n';
  } else {
    os << "<Probe pointer is expired>" << '\n';
  }
  os << indent << "TransmitWave: ";
  m_transmitWave.printSelf(os, indent + UFF_STD_INDENT);
}

}  // namespace uff