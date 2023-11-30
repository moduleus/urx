/*!
 * Copyright Moduleus
 * \file uff/position.h
 * \brief
 */

#ifndef UFF_POSTITION_H
#define UFF_POSTITION_H

#include "uff.h"

#include <iostream>

namespace uff::v0_5 {

/**
 * @brief Contains a location in space in Cartesian coordinates and SI units.
 */
class Position {
 public:
  explicit Position(MetadataType x = 0.f, MetadataType y = 0.f, MetadataType z = 0.f) {
    this->setX(x);
    this->setY(y);
    this->setZ(z);
  }

  friend std::ostream& operator<<(std::ostream& os, const uff::Position& r);

  MetadataType x() const { return m_x; }
  void setX(MetadataType x) { m_x = x; }

  MetadataType y() const { return m_y; }
  void setY(MetadataType y) { m_x = y; }

  MetadataType z() const { return m_z; }
  void setZ(MetadataType z) { m_z = z; }

 private:
  MetadataType m_x;
  MetadataType m_y;
  MetadataType m_z;
};

}  // namespace uff::v0_5

#endif  // UFF_POSTITION_H