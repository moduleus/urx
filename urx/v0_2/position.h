/*!
 * Copyright Moduleus
 * \file urx/position.h
 * \brief
 */

#ifndef URX_POSTITION_H
#define URX_POSTITION_H

#include <iosfwd>

#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

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

  friend std::ostream& operator<<(std::ostream& os, const Position& p);

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

}  // namespace urx::v0_2

#endif  // URX_POSTITION_H