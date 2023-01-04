/*!
 * Copyright Moduleus
 * \file uff/transform.cpp
 * \brief
 */

#include "uff/transform.h"

#include <ostream>
#include <utility>

namespace uff {

void Transform::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Rotation: ";
  this->rotation().printSelf(os, indent + UFF_STD_INDENT);

  os << indent << "Translation: ";
  this->translation().printSelf(os, indent + UFF_STD_INDENT);
}

}  // namespace uff