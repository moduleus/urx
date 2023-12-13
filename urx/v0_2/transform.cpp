// Copyright Moduleus

#include <ios>
#include <ostream>
#include <utility>

#include <urx/v0_2/object.h>
#include <urx/v0_2/rotation.h>
#include <urx/v0_2/transform.h>
#include <urx/v0_2/translation.h>

namespace urx::v0_2 {

void Transform::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Rotation: ";
  this->rotation().printSelf(os, indent + URX_STD_INDENT);

  os << indent << "Translation: ";
  this->translation().printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_2