/*!
 * Copyright Moduleus
 * \file urx/rca_array.cpp
 * \brief
 */

#include <urx/v0_5/rca_array.h>

#include <ostream>
#include <utility>

#include <urx/v0_5/probe.h>

namespace urx::v0_5 {

void RcaArray::printSelf(std::ostream& os, const std::string& indent) const {
  urx::Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + URX_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + URX_STD_INDENT << "NumberElementsX: " << m_numberElementsX << std::endl;
  os << indent + URX_STD_INDENT << "NumberElementsY: " << m_numberElementsY << std::endl;

  os << indent + URX_STD_INDENT << "PitchX: " << m_pitchX << std::endl;
  os << indent + URX_STD_INDENT << "PitchY: " << m_pitchY << std::endl;

  if (m_elementWidthX.has_value())
    os << indent + URX_STD_INDENT << "ElementWidthX: " << m_elementWidthX.value() << std::endl;
  else
    os << indent + URX_STD_INDENT << "ElementWidthX: " << UNDEFINED << std::endl;

  if (m_elementHeightX.has_value())
    os << indent + URX_STD_INDENT << "ElementHeightX: " << m_elementHeightX.value() << std::endl;
  else
    os << indent + URX_STD_INDENT << "ElementHeightX: " << UNDEFINED << std::endl;

  if (m_elementWidthY.has_value())
    os << indent + URX_STD_INDENT << "ElementWidthY: " << m_elementWidthY.value() << std::endl;
  else
    os << indent + URX_STD_INDENT << "ElementWidthY: " << UNDEFINED << std::endl;

  if (m_elementHeightY.has_value())
    os << indent + URX_STD_INDENT << "ElementHeightXY: " << m_elementHeightY.value() << std::endl;
  else
    os << indent + URX_STD_INDENT << "ElementHeightXY: " << UNDEFINED << std::endl;
}

}  // namespace urx::v0_5