/*!
 * Copyright Moduleus
 * \file urx/excitation.cpp
 * \brief
 */

#include <ios>
#include <ostream>

#include <urx/v0_5/excitation.h>
#include <urx/v0_5/object.h>

namespace urx::v0_5 {

void Excitation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  if (m_pulseShape.has_value())
    os << indent << "PulseShape: \"" << m_pulseShape.value() << '"' << '\n';
  else
    os << indent << "PulseShape: \"" << UNDEFINED << '"' << '\n';

  os << indent << "WaveForm: (size=" << m_waveform.size() << ")" << '\n';

  if (m_samplingFrequency.has_value())
    os << indent << "SamplingFrequency: " << m_samplingFrequency.value() << '\n';
  else
    os << indent << "SamplingFrequency: " << UNDEFINED << '"' << '\n';
}

}  // namespace urx::v0_5