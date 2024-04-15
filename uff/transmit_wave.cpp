/*!
 * Copyright Moduleus
 * \file uff/transmit_wave.cpp
 * \brief
 */

#include "uff/transmit_wave.h"

#include <ostream>
#include <string>

namespace uff {

void TransmitWave::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Wave: ";
  if (auto wpt = this->wave().lock()) {  // Has to be copied into a shared_ptr before usage
    //os << *wpt << '\n';
    (*wpt).printSelf(os, indent + UFF_STD_INDENT);
  } else {
    os << " <Wave pointer is expired>" << '\n';
  }
  os << indent << "TimeOffset: " << this->timeOffset()
     << '\n'
     //<< indent << "SamplingFrequency: " << this->samplingFrequency() << '\n'
     << indent << "Weight: " << this->weight() << '\n';
}

}  // namespace uff