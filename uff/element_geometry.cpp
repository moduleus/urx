/*!
 * Copyright Moduleus
 * \file uff/element_geometry.cpp
 * \brief
 */

#include "uff/element_geometry.h"

namespace uff {

void ElementGeometry::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Perimeter:\n";
  this->m_perimeter.printSelf(os, indent + UFF_STD_INDENT);
}

}  // namespace uff