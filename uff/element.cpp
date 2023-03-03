/*!
 * Copyright Moduleus
 * \file uff/element.cpp
 * \brief
 */

#include <uff/element.h>
#include <uff/object.h>
#include <ostream>

namespace uff {

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

}  // namespace uff