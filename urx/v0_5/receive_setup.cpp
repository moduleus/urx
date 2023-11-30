/*!
 * Copyright Moduleus
 * \file urx/receive_setup.cpp
 * \brief
 */

#include <urx/v0_5/receive_setup.h>

#include <ostream>

namespace urx::v0_5 {

void ReceiveSetup::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Probe: ";
  if (const auto spt = m_probe.lock()) {  // Has to be copied into a shared_ptr before usage
    os << &(*spt) << std::endl;
  } else {
    os << "<Probe pointer is expired>" << std::endl;
  }
  os << indent << "TimeOffset: " << m_timeOffset << std::endl;
  os << indent << "SamplingFrequency: " << m_samplingFrequency << std::endl;
}

}  // namespace urx::v0_5