/*!
 * Copyright Moduleus
 * \file urx/rca_array.cpp
 * \brief
 */

#include <cstddef>
#include <ios>
#include <ostream>
#include <utility>
#include <vector>

#include <urx/v0_5/element.h>
#include <urx/v0_5/object.h>
#include <urx/v0_5/probe.h>
#include <urx/v0_5/rca_array.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

void RcaArray::printSelf(std::ostream& os, const std::string& indent) const {
  Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + URX_STD_INDENT);  // NOLINT(bugprone-parent-virtual-call)
  os << indent + URX_STD_INDENT << "NumberElementsX: " << m_numberElementsX << '\n';
  os << indent + URX_STD_INDENT << "NumberElementsY: " << m_numberElementsY << '\n';

  os << indent + URX_STD_INDENT << "PitchX: " << m_pitchX << '\n';
  os << indent + URX_STD_INDENT << "PitchY: " << m_pitchY << '\n';

  if (m_elementWidthX.has_value())
    os << indent + URX_STD_INDENT << "ElementWidthX: " << m_elementWidthX.value() << '\n';
  else
    os << indent + URX_STD_INDENT << "ElementWidthX: " << UNDEFINED << '\n';

  if (m_elementHeightX.has_value())
    os << indent + URX_STD_INDENT << "ElementHeightX: " << m_elementHeightX.value() << '\n';
  else
    os << indent + URX_STD_INDENT << "ElementHeightX: " << UNDEFINED << '\n';

  if (m_elementWidthY.has_value())
    os << indent + URX_STD_INDENT << "ElementWidthY: " << m_elementWidthY.value() << '\n';
  else
    os << indent + URX_STD_INDENT << "ElementWidthY: " << UNDEFINED << '\n';

  if (m_elementHeightY.has_value())
    os << indent + URX_STD_INDENT << "ElementHeightXY: " << m_elementHeightY.value() << '\n';
  else
    os << indent + URX_STD_INDENT << "ElementHeightXY: " << UNDEFINED << '\n';
}

void RcaArray::updateElements() {
  m_elements.resize(static_cast<size_t>(m_numberElementsX) + m_numberElementsY);

  const MetadataType xmin = -m_pitchX * (m_numberElementsX - 1.f) / 2.f;

  for (uint32_t i = 0; i < m_numberElementsX; i++) {
    Element element;
    element.setX(xmin + i * m_pitchX);
    element.setY(0.f);
    element.setZ(0.f);
    m_elements[i] = element;
  }

  const MetadataType ymin = -m_pitchY * (m_numberElementsY - 1.f) / 2.f;
  for (uint32_t i = m_numberElementsX; i < m_elements.size(); i++) {
    Element element;
    element.setY(ymin + (i - m_numberElementsX) * m_pitchY);
    element.setX(0.f);
    element.setZ(0.f);
    m_elements[i] = element;
  }
}

}  // namespace urx::v0_5