/*!
 * Copyright Moduleus
 * \file uff_aperture.cpp
 * \brief
 */

#include "uff_aperture.h"

namespace uff {

void Aperture::printSelf(std::ostream& os, std::string indent) const {
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

}  // namespace uff