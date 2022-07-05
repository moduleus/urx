/*!
 * Copyright Moduleus
 * \file uff_rca_array.cpp
 * \brief
 */

#include "uff_rca_array.h"

namespace uff {

void RcaArray::printSelf(std::ostream& os, std::string indent) const {
  uff::Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + UFF_STD_INDENT);
  os << indent + UFF_STD_INDENT << "NumberElementsX: " << m_numberElementsX << std::endl;
  os << indent + UFF_STD_INDENT << "NumberElementsY: " << m_numberElementsY << std::endl;

  if (m_pitchX.has_value())
    os << indent + UFF_STD_INDENT << "PitchX: " << m_pitchX.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "PitchX: " << UNDEFINED << std::endl;

  if (m_pitchY.has_value())
    os << indent + UFF_STD_INDENT << "PitchY: " << m_pitchY.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "PitchY: " << UNDEFINED << std::endl;

  if (m_elementWidthX.has_value())
    os << indent + UFF_STD_INDENT << "ElementWidthX: " << m_elementWidthX.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "ElementWidthX: " << UNDEFINED << std::endl;

  if (m_elementHeightX.has_value())
    os << indent + UFF_STD_INDENT << "ElementHeightX: " << m_elementHeightX.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "ElementHeightX: " << UNDEFINED << std::endl;

  if (m_elementWidthY.has_value())
    os << indent + UFF_STD_INDENT << "ElementWidthY: " << m_elementWidthY.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "ElementWidthY: " << UNDEFINED << std::endl;

  if (m_elementHeightY.has_value())
    os << indent + UFF_STD_INDENT << "ElementHeightXY: " << m_elementHeightY.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "ElementHeightXY: " << UNDEFINED << std::endl;
}

}  // namespace uff