/*!
 * Copyright Moduleus
 * \file urx/linear_array.cpp
 * \brief
 */

#include <ostream>
#include <utility>

#include <urx/v0_5/linear_array.h>
#include <urx/v0_5/probe.h>

namespace urx::v0_5 {

void LinearArray::printSelf(std::ostream& os, const std::string& indent) const {
  Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + URX_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + URX_STD_INDENT << "NumberElements: " << m_numberElements << std::endl;
  os << indent + URX_STD_INDENT << "Pitch: " << m_pitch << std::endl;
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