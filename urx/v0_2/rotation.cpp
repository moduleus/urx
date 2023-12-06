// Copyright Moduleus

#include <ios>
#include <ostream>

#include <urx/v0_2/rotation.h>

namespace urx::v0_2 {

void Rotation::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "X: " << this->x() << ", "
     << "Y: " << this->y() << ", "
     << "Z: " << this->z() << '\n';
}

}  // namespace urx::v0_2