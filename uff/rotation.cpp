/*!
 * Copyright Moduleus
 * \file uff/rotation.cpp
 * \brief
 */

#include "uff/rotation.h"

namespace uff {

void Rotation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "X: " << this->x() << ", "
     << "Y: " << this->y() << ", "
     << "Z: " << this->z() << std::endl;
}

}  // namespace uff