/*!
 * Copyright Moduleus
 * \file uff/linear_array.cpp
 * \brief
 */

#include "uff/v0_5/linear_array.h"

#include <ostream>
#include <utility>

#include "uff/v0_5/probe.h"

namespace uff::v0_5 {

void LinearArray::printSelf(std::ostream& os, const std::string& indent) const {
  uff::Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + UFF_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + UFF_STD_INDENT << "NumberElements: " << m_numberElements << std::endl;
  os << indent + UFF_STD_INDENT << "Pitch: " << m_pitch << std::endl;
  if (m_elementWidth.has_value())
    os << indent + UFF_STD_INDENT << "ElementWidth: " << m_elementWidth.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "ElementWidth: " << UNDEFINED << std::endl;

  if (m_elementHeight.has_value())
    os << indent + UFF_STD_INDENT << "ElementHeight: " << m_elementHeight.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "ElementHeight: " << UNDEFINED << std::endl;
}

}  // namespace uff::v0_5