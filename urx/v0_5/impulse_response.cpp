/*!
 * Copyright Moduleus
 * \file urx/impulse_response.cpp
 * \brief
 */

#include <ostream>

#include <urx/v0_5/impulse_response.h>

namespace urx::v0_5 {

void ImpulseResponse::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "InitialTime: " << m_initialTime << std::endl;

  if (m_samplingFrequency.has_value())
    os << indent << "SamplingFrequency: " << m_samplingFrequency.value() << std::endl;
  else
    os << indent << "SamplingFrequency: " << UNDEFINED << std::endl;

  os << indent << "Units: " << units() << std::endl;
  os << indent << "Data: "
     << "TODO" << std::endl;
}

}  // namespace urx::v0_5