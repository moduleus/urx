/*!
 * Copyright Moduleus
 * \file uff/translation.cpp
 * \brief
 */

#include "uff/v0_5/translation.h"

#include <ostream>

namespace uff::v0_5 {

void Translation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "X: " << this->x() << ", "
     << "Y: " << this->y() << ", "
     << "Z: " << this->z() << std::endl;
}

}  // namespace uff::v0_5
