/*!
 * Copyright Moduleus
 * \file urx/element_geometry.cpp
 * \brief
 */

#include <ostream>
#include <utility>

#include <urx/v0_5/element_geometry.h>

namespace urx::v0_5 {

void ElementGeometry::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Perimeter:\n";
  this->m_perimeter.printSelf(os, indent + URX_STD_INDENT);
}

}  // namespace urx::v0_5