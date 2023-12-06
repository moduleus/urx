/*!
 * Copyright Moduleus
 * \file urx/element.cpp
 * \brief
 */

#include <ios>
#include <ostream>

#include <urx/v0_2/element.h>
#include <urx/v0_2/object.h>

namespace urx::v0_2 {

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
  os << '\n';
}

}  // namespace urx::v0_2