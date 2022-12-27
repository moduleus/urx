/*!
 * Copyright Moduleus
 * \file uff/event.cpp
 * \brief
 */

#include "uff/event.h"

namespace uff {

void Event::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);

  os << indent << "TransmitSetup: ";
  this->m_transmitSetup.printSelf(os, indent + UFF_STD_INDENT);

  os << indent << "ReceiveSetup: ";
  this->m_receiveSetup.printSelf(os, indent + UFF_STD_INDENT);
}

}  // namespace uff