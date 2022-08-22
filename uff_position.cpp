/*!
 * Copyright Moduleus
 * \file uff_position.cpp
 * \brief
 */

#include "uff_position.h"

namespace uff {

std::ostream& operator<<(std::ostream& os, const uff::Position& p) {
  os << "Position(" << p.x() << ", " << p.y() << ", " << p.z() << ')';
  return os;
}

}  // namespace uff