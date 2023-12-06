// Copyright Moduleus

#include <ios>
#include <ostream>

#include <urx/v0_2/object.h>
#include <urx/v0_2/transmit_wave.h>
#include <urx/v0_2/wave.h>

namespace urx::v0_2 {

void TransmitWave::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Wave: ";
  if (auto wpt = this->wave().lock()) {  // Has to be copied into a shared_ptr before usage
    //os << *wpt << '\n';
    (*wpt).printSelf(os, indent + URX_STD_INDENT);
  } else {
    os << " <Wave pointer is expired>" << '\n';
  }
  os << indent << "TimeOffset: " << this->timeOffset()
     << '\n'
     //<< indent << "SamplingFrequency: " << this->samplingFrequency() << '\n'
     << indent << "Weight: " << this->weight() << '\n';
}

}  // namespace urx::v0_2