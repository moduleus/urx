/*!
 * Copyright Moduleus
 * \file urx/impulse_response.cpp
 * \brief
 */

#include <ios>
#include <ostream>

#include <urx/v0_5/impulse_response.h>
#include <urx/v0_5/object.h>

namespace urx::v0_5 {

void ImpulseResponse::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "InitialTime: " << m_initialTime << '\n';

  if (m_samplingFrequency.has_value())
    os << indent << "SamplingFrequency: " << m_samplingFrequency.value() << '\n';
  else
    os << indent << "SamplingFrequency: " << UNDEFINED << '\n';

  os << indent << "Units: " << units() << '\n';
  os << indent << "Data: "
     << "TODO" << '\n';
}

}  // namespace urx::v0_5