/*!
 * Copyright Moduleus
 * \file uff/matrix_array.cpp
 * \brief
 */

#include "uff/matrix_array.h"

#include <ostream>
#include <string>
#include <utility>

#include "uff/probe.h"

namespace uff {

void MatrixArray::printSelf(std::ostream& os, const std::string& indent) const {
  uff::Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + UFF_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + UFF_STD_INDENT << "NumberElementsX: " << m_numberElementsX << '\n';
  os << indent + UFF_STD_INDENT << "NumberElementsY: " << m_numberElementsY << '\n';

  os << indent + UFF_STD_INDENT << "PitchX: " << m_pitchX << '\n';
  os << indent + UFF_STD_INDENT << "PitchY: " << m_pitchY << '\n';

  if (m_elementWidth.has_value())
    os << indent + UFF_STD_INDENT << "ElementWidth: " << m_elementWidth.value() << '\n';
  else
    os << indent + UFF_STD_INDENT << "ElementWidth: " << UNDEFINED << '\n';

  if (m_elementHeight.has_value())
    os << indent + UFF_STD_INDENT << "ElementHeight: " << m_elementHeight.value() << '\n';
  else
    os << indent + UFF_STD_INDENT << "ElementHeight: " << UNDEFINED << '\n';
}

}  // namespace uff