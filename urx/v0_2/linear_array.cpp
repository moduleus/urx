/*!
 * Copyright Moduleus
 * \file urx/linear_array.cpp
 * \brief
 */

#include <ios>
#include <ostream>
#include <utility>

#include <urx/v0_2/linear_array.h>
#include <urx/v0_2/object.h>
#include <urx/v0_2/probe.h>

namespace urx::v0_2 {

void LinearArray::printSelf(std::ostream& os, const std::string& indent) const {
  Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + URX_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + URX_STD_INDENT << "NumberElements: " << m_numberElements << '\n';
  os << indent + URX_STD_INDENT << "Pitch: " << m_pitch << '\n';
  if (m_elementWidth.has_value())
    os << indent + URX_STD_INDENT << "ElementWidth: " << m_elementWidth.value() << '\n';
  else
    os << indent + URX_STD_INDENT << "ElementWidth: " << UNDEFINED << '\n';

  if (m_elementHeight.has_value())
    os << indent + URX_STD_INDENT << "ElementHeight: " << m_elementHeight.value() << '\n';
  else
    os << indent + URX_STD_INDENT << "ElementHeight: " << UNDEFINED << '\n';
}

}  // namespace urx::v0_2