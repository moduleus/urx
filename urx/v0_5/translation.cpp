/*!
 * Copyright Moduleus
 * \file urx/translation.cpp
 * \brief
 */

#include <ostream>

#include <urx/v0_5/translation.h>

namespace urx::v0_5 {

void Translation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "X: " << this->x() << ", "
     << "Y: " << this->y() << ", "
     << "Z: " << this->z() << std::endl;
}

}  // namespace urx::v0_5
