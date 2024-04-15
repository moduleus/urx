/*!
 * Copyright Moduleus
 * \file uff/version.cpp
 * \brief
 */

#include "uff/version.h"

#include <ostream>
#include <string>

namespace uff {

void Version::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Major: " << this->m_major << '\n'
     << indent << "Minor: " << this->m_minor << '\n'
     << indent << "Patch: " << this->m_patch << '\n';
}

}  // namespace uff