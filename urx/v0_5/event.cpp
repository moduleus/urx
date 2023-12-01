/*!
 * Copyright Moduleus
 * \file urx/event.cpp
 * \brief
 */

#include <ios>
#include <ostream>
#include <utility>

#include <urx/v0_5/event.h>
#include <urx/v0_5/object.h>
#include <urx/v0_5/receive_setup.h>
#include <urx/v0_5/transmit_setup.h>

namespace urx::v0_5 {

void Event::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);

  os << indent << "TransmitSetup: ";
  this->m_transmitSetup.printSelf(os, indent + URX_STD_INDENT);

  os << indent << "ReceiveSetup: ";
  this->m_receiveSetup.printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_5