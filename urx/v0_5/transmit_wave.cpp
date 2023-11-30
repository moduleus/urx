/*!
 * Copyright Moduleus
 * \file urx/transmit_wave.cpp
 * \brief
 */

#include <urx/v0_5/transmit_wave.h>

#include <ostream>

namespace urx::v0_5 {

void TransmitWave::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Wave: ";
  if (auto wpt = this->wave().lock()) {  // Has to be copied into a shared_ptr before usage
    //os << *wpt << std::endl;
    (*wpt).printSelf(os, indent + URX_STD_INDENT);
  } else {
    os << " <Wave pointer is expired>" << std::endl;
  }
  os << indent << "TimeOffset: " << this->timeOffset()
     << std::endl
     //<< indent << "SamplingFrequency: " << this->samplingFrequency() << std::endl
     << indent << "Weight: " << this->weight() << std::endl;
}

}  // namespace urx::v0_5