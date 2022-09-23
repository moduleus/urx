/*!
 * Copyright Moduleus
 * \file uff/position.cpp
 * \brief
 */

#include "uff/position.h"

namespace uff {

std::ostream& operator<<(std::ostream& os, const uff::Position& p) {
  os << "Position(" << p.x() << ", " << p.y() << ", " << p.z() << ')';
  return os;
}

}  // namespace uff