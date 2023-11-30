/*!
 * Copyright Moduleus
 * \file urx/element.cpp
 * \brief
 */

#include <urx/v0_5/element.h>

#include <ostream>

namespace urx::v0_5 {

void Element::printSelf(std::ostream& os, const std::string& indent) const {
  //superclass::printSelf(os, indent);
  os << indent << "X: ";
  if (m_x.has_value())
    os << m_x.value();
  else
    os << UNDEFINED;

  os << ", Y: ";
  if (m_y.has_value())
    os << m_y.value();
  else
    os << UNDEFINED;

  os << ", Z: ";
  if (m_z.has_value())
    os << m_z.value();
  else
    os << UNDEFINED;
  os << std::endl;
}

}  // namespace urx::v0_5