/*!
 * Copyright Moduleus
 * \file urx/transform.cpp
 * \brief
 */

#include <ostream>
#include <utility>

#include <urx/v0_5/transform.h>

namespace urx::v0_5 {

void Transform::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Rotation: ";
  this->rotation().printSelf(os, indent + URX_STD_INDENT);

  os << indent << "Translation: ";
  this->translation().printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_5