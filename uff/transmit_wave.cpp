/*!
 * Copyright Moduleus
 * \file uff/transmit_wave.cpp
 * \brief
 */

#include <uff/object.h>
#include <uff/transmit_wave.h>
#include <uff/wave.h>
#include <ostream>

namespace uff {

void TransmitWave::printSelf(std::ostream& os, const std::string& indent) const {
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