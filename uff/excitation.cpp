/*!
 * Copyright Moduleus
 * \file uff/excitation.cpp
 * \brief
 */

#include "uff/excitation.h"

#include <ostream>
#include <string>

namespace uff {

void Excitation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  if (m_pulseShape.has_value())
    os << indent << "PulseShape: \"" << m_pulseShape.value() << '"' << '\n';
  else
    os << indent << "PulseShape: \"" << UNDEFINED << '"' << '\n';

  os << indent << "WaveForm: (size=" << m_waveform.size() << ")" << '\n';

  if (m_pulseShape.has_value())
    os << indent << "SamplingFrequency: " << m_samplingFrequency.value() << '\n';
  else
    os << indent << "SamplingFrequency: " << UNDEFINED << '"' << '\n';
}

}  // namespace uff