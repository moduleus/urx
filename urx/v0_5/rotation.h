/*!
 * Copyright Moduleus
 * \file urx/rotation.h
 * \brief
 */

#ifndef URX_ROTATION_H
#define URX_ROTATION_H

#include <iosfwd>
#include <string>

#include <urx/v0_5/object.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

/**
 * @brief The URX Rotation class contains a rotation in space in spherical coordinates and SI units. 
 * The rotation is specified using Euler angles that are applied in the order ZYX.
 */
class Rotation : public urx::Object {
  URX_TYPE_MACRO(Rotation, urx::Object);

 public:
  explicit Rotation(MetadataType rx = 0.f, MetadataType ry = 0.f, MetadataType rz = 0.f) {
    setX(rx);
    setY(ry);
    setZ(rz);
  }

  void printSelf(std::ostream& os, const std::string& indent) const override;

  MetadataType x() const { return m_rx; }
  void setX(MetadataType x) { m_rx = x; }

  MetadataType y() const { return m_ry; }
  void setY(MetadataType y) { m_ry = y; }

  MetadataType z() const { return m_rz; }
  void setZ(MetadataType z) { m_rz = z; }

  MetadataType elevation() const { return m_rx; }
  MetadataType azimut() const { return m_ry; }
  MetadataType roll() const { return m_rz; }

  bool operator==(const Rotation& other) const {
    return ((m_rx == other.m_rx) && (m_ry == other.m_ry) && (m_rz == other.m_rz));
  }

  inline bool operator!=(const Rotation& other) const { return !(*this == other); }

 private:
  // Rotation around the X-axis (elevation) in radians
  MetadataType m_rx;

  // Rotation around the Y-axis (azimut) in radians
  MetadataType m_ry;

  // Rotation around the Z-axis (roll) in radians
  MetadataType m_rz;
};

}  // namespace urx::v0_5

#endif  // URX_ROTATION_H