/*!
 * Copyright Moduleus
 * \file urx/timed_event.cpp
 * \brief
 */

#include <ios>
#include <ostream>

#include <urx/v0_5/timed_event.h>

namespace urx::v0_5 {

void TimedEvent::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "TimeOffset: " << this->timeOffset() << '\n' << indent << "Event: ";
  if (auto spt = this->evenement().lock()) {  // Has to be copied into a shared_ptr before usage
    os << "0x" << &(*spt) << '\n';
  } else {
    os << "<Event pointer is expired>" << '\n';
  }
}

}  // namespace urx::v0_5