/*!
 * Copyright Moduleus
 * \file uff/timed_event.cpp
 * \brief
 */

#include "uff/timed_event.h"

#include <ostream>
#include <string>

namespace uff {

void TimedEvent::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "TimeOffset: " << this->timeOffset() << '\n' << indent << "Event: ";
  if (auto spt = this->evenement().lock()) {  // Has to be copied into a shared_ptr before usage
    os << "0x" << &(*spt) << '\n';
  } else {
    os << "<Event pointer is expired>" << '\n';
  }
}

}  // namespace uff