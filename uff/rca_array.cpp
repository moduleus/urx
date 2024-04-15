/*!
 * Copyright Moduleus
 * \file uff/rca_array.cpp
 * \brief
 */

#include "uff/rca_array.h"

#include <ostream>
#include <string>
#include <utility>

#include "uff/probe.h"

namespace uff {

void RcaArray::printSelf(std::ostream& os, const std::string& indent) const {
  uff::Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + UFF_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + UFF_STD_INDENT << "NumberElementsX: " << m_numberElementsX << '\n';
  os << indent + UFF_STD_INDENT << "NumberElementsY: " << m_numberElementsY << '\n';

  os << indent + UFF_STD_INDENT << "PitchX: " << m_pitchX << '\n';
  os << indent + UFF_STD_INDENT << "PitchY: " << m_pitchY << '\n';

  if (m_elementWidthX.has_value())
    os << indent + UFF_STD_INDENT << "ElementWidthX: " << m_elementWidthX.value() << '\n';
  else
    os << indent + UFF_STD_INDENT << "ElementWidthX: " << UNDEFINED << '\n';

  if (m_elementHeightX.has_value())
    os << indent + UFF_STD_INDENT << "ElementHeightX: " << m_elementHeightX.value() << '\n';
  else
    os << indent + UFF_STD_INDENT << "ElementHeightX: " << UNDEFINED << '\n';

  if (m_elementWidthY.has_value())
    os << indent + UFF_STD_INDENT << "ElementWidthY: " << m_elementWidthY.value() << '\n';
  else
    os << indent + UFF_STD_INDENT << "ElementWidthY: " << UNDEFINED << '\n';

  if (m_elementHeightY.has_value())
    os << indent + UFF_STD_INDENT << "ElementHeightXY: " << m_elementHeightY.value() << '\n';
  else
    os << indent + UFF_STD_INDENT << "ElementHeightXY: " << UNDEFINED << '\n';
}

}  // namespace uff