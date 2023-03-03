/*!
 * Copyright Moduleus
 * \file uff/translation.cpp
 * \brief
 */

#include <uff/translation.h>
#include <ostream>

namespace uff {

void Translation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "X: " << this->x() << ", "
     << "Y: " << this->y() << ", "
     << "Z: " << this->z() << std::endl;
}

}  // namespace uff
