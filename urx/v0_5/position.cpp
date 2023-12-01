/*!
 * Copyright Moduleus
 * \file urx/position.cpp
 * \brief
 */

#include <ios>
#include <ostream>
#include <string>

#include <urx/v0_5/position.h>

namespace urx::v0_5 {

std::ostream& operator<<(std::ostream& os, const Position& p) {
  os << "Position(" << p.x() << ", " << p.y() << ", " << p.z() << ')';
  return os;
}

}  // namespace urx::v0_5