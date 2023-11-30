/*!
 * Copyright Moduleus
 * \file uff/version.cpp
 * \brief
 */

#include "uff/v0_5/version.h"

#include <ostream>

namespace uff::v0_5 {

void Version::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Major: " << this->m_major << std::endl
     << indent << "Minor: " << this->m_minor << std::endl
     << indent << "Patch: " << this->m_patch << std::endl;
}

}  // namespace uff::v0_5