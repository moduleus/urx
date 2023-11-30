/*!
 * Copyright Moduleus
 * \file uff/rotation.cpp
 * \brief
 */

#include "uff/v0_5/rotation.h"

#include <ostream>

namespace uff::v0_5 {

void Rotation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "X: " << this->x() << ", "
     << "Y: " << this->y() << ", "
     << "Z: " << this->z() << std::endl;
}

}  // namespace uff::v0_5