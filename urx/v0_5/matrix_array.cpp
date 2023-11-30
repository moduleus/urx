/*!
 * Copyright Moduleus
 * \file urx/matrix_array.cpp
 * \brief
 */

#include <ostream>
#include <utility>

#include <urx/v0_5/matrix_array.h>
#include <urx/v0_5/probe.h>

namespace urx::v0_5 {

void MatrixArray::printSelf(std::ostream& os, const std::string& indent) const {
  Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + URX_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + URX_STD_INDENT << "NumberElementsX: " << m_numberElementsX << std::endl;
  os << indent + URX_STD_INDENT << "NumberElementsY: " << m_numberElementsY << std::endl;

  os << indent + URX_STD_INDENT << "PitchX: " << m_pitchX << std::endl;
  os << indent + URX_STD_INDENT << "PitchY: " << m_pitchY << std::endl;

  if (m_elementWidth.has_value())
    os << indent + URX_STD_INDENT << "ElementWidth: " << m_elementWidth.value() << std::endl;
  else
    os << indent + URX_STD_INDENT << "ElementWidth: " << UNDEFINED << std::endl;

  if (m_elementHeight.has_value())
    os << indent + URX_STD_INDENT << "ElementHeight: " << m_elementHeight.value() << std::endl;
  else
    os << indent + URX_STD_INDENT << "ElementHeight: " << UNDEFINED << std::endl;
}

}  // namespace urx::v0_5