// Copyright Moduleus

#ifndef URX_TRANSFORM_H
#define URX_TRANSFORM_H

#include <iosfwd>
#include <string>

#include <urx/v0_2/object.h>
#include <urx/v0_2/rotation.h>
#include <urx/v0_2/translation.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief The URX Transform class
 *  Defines an affine transformation in a 3D Cartesian system.
 *  A transform is composed by a rotation R = (rx, ry, rz)
 *  followed by a translation T = (tx, ty, tz).
 */
class Transform : public Object {
  URX_TYPE_MACRO(Transform, Object);

 public:
  Transform() = default;

  Transform(const Rotation& rotation, const Translation& translation) {
    this->setRotation(rotation);
    this->setTranslation(translation);
  }

  Transform(Transform const& other) = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  const Rotation& rotation() const { return m_rotation; }
  void setRotation(const Rotation& rotation) { m_rotation = rotation; }

  const Translation& translation() const { return m_translation; }
  void setTranslation(const Translation& translation) { m_translation = translation; }

  bool operator==(const Transform& other) const {
    return ((m_rotation == other.m_rotation) && (m_translation == other.m_translation));
  }

  inline bool operator!=(const Transform& other) const { return !(*this == other); }

  Transform& operator=(const Transform& other) = default;

 private:
  // rotation in radians
  Rotation m_rotation;

  // position in meters
  Translation m_translation;
};

}  // namespace urx::v0_2

#endif  // URX_TRANSFORM_H