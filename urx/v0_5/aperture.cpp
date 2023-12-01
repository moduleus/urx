/*!
 * Copyright Moduleus
 * \file urx/aperture.cpp
 * \brief
 */

#include <ios>
#include <ostream>

#include <urx/v0_5/aperture.h>
#include <urx/v0_5/object.h>

namespace urx::v0_5 {

void Aperture::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  if (m_window.has_value())
    os << indent << "Window: \"" << m_window.value() << "\"" << '\n';
  else
    os << indent << "Window: \"" << UNDEFINED << "\"" << '\n';

  if (m_fNumber.has_value())
    os << indent << "F-Number: " << m_fNumber.value() << '\n';
  else
    os << indent << "F-Number: " << UNDEFINED << '\n';

  if (m_fixedSize.has_value())
    os << indent << "FixedSize: " << m_fixedSize.value() << '\n';
  else
    os << indent << "FixedSize: " << UNDEFINED << '\n';
}

}  // namespace urx::v0_5