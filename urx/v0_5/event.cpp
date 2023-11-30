/*!
 * Copyright Moduleus
 * \file urx/event.cpp
 * \brief
 */

#include <urx/v0_5/event.h>

#include <ostream>
#include <utility>

namespace urx::v0_5 {

void Event::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);

  os << indent << "TransmitSetup: ";
  this->m_transmitSetup.printSelf(os, indent + URX_STD_INDENT);

  os << indent << "ReceiveSetup: ";
  this->m_receiveSetup.printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_5