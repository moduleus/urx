/*!
 * Copyright Moduleus
 * \file uff/perimeter.cpp
 * \brief
 */

#include "uff/v0_5/perimeter.h"

#include <ostream>

namespace uff::v0_5 {

void Perimeter::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Points: (";
  for (const auto& pt : this->m_points) {
    os << "(" << pt.first << ", " << pt.second << "), ";
  }
  os << ")" << std::endl;
}

}  // namespace uff::v0_5