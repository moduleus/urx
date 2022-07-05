/*!
 * Copyright Moduleus
 * \file uff_transmit_wave.cpp
 * \brief
 */

#include "uff_transmit_wave.h"

namespace uff {

void TransmitWave::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Wave: ";
  if (auto wpt = this->wave().lock()) {  // Has to be copied into a shared_ptr before usage
    //os << *wpt << std::endl;
    (*wpt).printSelf(os, indent + UFF_STD_INDENT);
  } else {
    os << " <Wave pointer is expired>" << std::endl;
  }
  os << indent << "TimeOffset: " << this->timeOffset()
     << std::endl
     //<< indent << "SamplingFrequency: " << this->samplingFrequency() << std::endl
     << indent << "Weight: " << this->weight() << std::endl;
}

}  // namespace uff