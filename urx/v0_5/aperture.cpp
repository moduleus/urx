/*!
 * Copyright Moduleus
 * \file urx/aperture.cpp
 * \brief
 */

#include <urx/v0_5/aperture.h>

#include <ostream>

namespace urx::v0_5 {

void Aperture::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  if (m_window.has_value())
    os << indent << "Window: \"" << m_window.value() << "\"" << std::endl;
  else
    os << indent << "Window: \"" << UNDEFINED << "\"" << std::endl;

  if (m_fNumber.has_value())
    os << indent << "F-Number: " << m_fNumber.value() << std::endl;
  else
    os << indent << "F-Number: " << UNDEFINED << std::endl;

  if (m_fixedSize.has_value())
    os << indent << "FixedSize: " << m_fNumber.value() << std::endl;
  else
    os << indent << "FixedSize: " << UNDEFINED << std::endl;
}

}  // namespace urx::v0_5