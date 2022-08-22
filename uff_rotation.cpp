/*!
 * Copyright Moduleus
 * \file uff_rotation.cpp
 * \brief
 */

#include "uff_rotation.h"

namespace uff {

void Rotation::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
  os << indent << "X: " << this->x() << ", "
     << "Y: " << this->y() << ", "
     << "Z: " << this->z() << std::endl;
}

}  // namespace uff