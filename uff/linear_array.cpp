/*!
 * Copyright Moduleus
 * \file uff/linear_array.cpp
 * \brief
 */

#include "uff/linear_array.h"

namespace uff {

void LinearArray::printSelf(std::ostream& os, std::string indent) const {
  uff::Probe::printSelf(os, indent);
  os << indent;
  superclass::printSelf(os, indent + UFF_STD_INDENT);
  os << indent + UFF_STD_INDENT << "NumberElements: " << m_numberElements << std::endl;

  if (m_pitch.has_value())
    os << indent + UFF_STD_INDENT << "Pitch: " << m_pitch.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "Pitch: " << UNDEFINED << std::endl;

  if (m_elementWidth.has_value())
    os << indent + UFF_STD_INDENT << "ElementWidth: " << m_elementWidth.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "ElementWidth: " << UNDEFINED << std::endl;

  if (m_elementHeight.has_value())
    os << indent + UFF_STD_INDENT << "ElementHeight: " << m_elementHeight.value() << std::endl;
  else
    os << indent + UFF_STD_INDENT << "ElementHeight: " << UNDEFINED << std::endl;
}

}  // namespace uff