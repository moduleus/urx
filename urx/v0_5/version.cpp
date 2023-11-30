/*!
 * Copyright Moduleus
 * \file urx/version.cpp
 * \brief
 */

#include <urx/v0_5/version.h>

#include <ostream>

namespace urx::v0_5 {

void Version::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Major: " << this->m_major << std::endl
     << indent << "Minor: " << this->m_minor << std::endl
     << indent << "Patch: " << this->m_patch << std::endl;
}

}  // namespace urx::v0_5