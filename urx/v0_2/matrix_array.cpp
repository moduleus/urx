// Copyright Moduleus

#include <ios>
#include <ostream>
#include <utility>

#include <urx/v0_2/matrix_array.h>
#include <urx/v0_2/object.h>
#include <urx/v0_2/probe.h>

namespace urx::v0_2 {

void MatrixArray::printSelf(std::ostream& os, const std::string& indent) const {
  Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + URX_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + URX_STD_INDENT << "NumberElementsX: " << m_numberElementsX << '\n';
  os << indent + URX_STD_INDENT << "NumberElementsY: " << m_numberElementsY << '\n';

  os << indent + URX_STD_INDENT << "PitchX: " << m_pitchX << '\n';
  os << indent + URX_STD_INDENT << "PitchY: " << m_pitchY << '\n';

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