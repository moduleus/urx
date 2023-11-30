/*!
 * Copyright Moduleus
 * \file uff/position.cpp
 * \brief
 */

#include "uff/v0_5/position.h"

namespace uff::v0_5 {

std::ostream& operator<<(std::ostream& os, const uff::Position& p) {
  os << "Position(" << p.x() << ", " << p.y() << ", " << p.z() << ')';
  return os;
}

}  // namespace uff::v0_5