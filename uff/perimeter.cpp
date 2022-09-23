/*!
 * Copyright Moduleus
 * \file uff/perimeter.cpp
 * \brief
 */

#include "uff/perimeter.h"

namespace uff {

void Perimeter::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Points: (";
  for (const auto& pt : this->m_points) {
    os << "(" << pt.first << ", " << pt.second << "), ";
  }
  os << ")" << std::endl;
}

}  // namespace uff