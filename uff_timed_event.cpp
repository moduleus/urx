/*!
 * Copyright Moduleus
 * \file uff_timed_event.cpp
 * \brief
 */

#include "uff_timed_event.h"

namespace uff {

void TimedEvent::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
  os << indent << "TimeOffset: " << this->timeOffset() << std::endl << indent << "Event: ";
  if (auto spt = this->evenement().lock()) {  // Has to be copied into a shared_ptr before usage
    os << "0x" << &(*spt) << std::endl;
  } else {
    os << "<Event pointer is expired>" << std::endl;
  }
}

}  // namespace uff