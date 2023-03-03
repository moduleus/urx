/*!
 * Copyright Moduleus
 * \file uff/excitation.cpp
 * \brief
 */

#include <uff/excitation.h>
#include <uff/object.h>
#include <ostream>

namespace uff {

void Excitation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  if (m_pulseShape.has_value())
    os << indent << "PulseShape: \"" << m_pulseShape.value() << '"' << std::endl;
  else
    os << indent << "PulseShape: \"" << UNDEFINED << '"' << std::endl;

  os << indent << "WaveForm: (size=" << m_waveform.size() << ")" << std::endl;

  if (m_pulseShape.has_value())
    os << indent << "SamplingFrequency: " << m_samplingFrequency.value() << std::endl;
  else
    os << indent << "SamplingFrequency: " << UNDEFINED << '"' << std::endl;
}

}  // namespace uff