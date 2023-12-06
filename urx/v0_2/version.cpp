/*!
 * Copyright Moduleus
 * \file urx/version.cpp
 * \brief
 */

#include <ios>
#include <ostream>

#include <urx/v0_2/version.h>

namespace urx::v0_2 {

void Version::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Major: " << this->m_major << '\n'
     << indent << "Minor: " << this->m_minor << '\n'
     << indent << "Patch: " << this->m_patch << '\n';
}

}  // namespace urx::v0_2