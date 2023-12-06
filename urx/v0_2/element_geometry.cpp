// Copyright Moduleus

#include <ios>
#include <ostream>
#include <utility>

#include <urx/v0_2/element_geometry.h>
#include <urx/v0_2/object.h>
#include <urx/v0_2/perimeter.h>

namespace urx::v0_2 {

void ElementGeometry::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Perimeter:\n";
  this->m_perimeter.printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_2