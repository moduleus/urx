/*!
 * Copyright Moduleus
 * \file urx/rotation.cpp
 * \brief
 */

#include <ostream>

#include <urx/v0_5/rotation.h>

namespace urx::v0_5 {

void Rotation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "X: " << this->x() << ", "
     << "Y: " << this->y() << ", "
     << "Z: " << this->z() << std::endl;
}

}  // namespace urx::v0_5