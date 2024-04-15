/*!
 * Copyright Moduleus
 * \file uff/impulse_response.cpp
 * \brief
 */

#include "uff/impulse_response.h"

#include <ostream>
#include <string>

namespace uff {

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

}  // namespace uff