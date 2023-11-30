/*!
 * Copyright Moduleus
 * \file urx/transform.h
 * \brief
 */

#ifndef URX_TRANSFORM_H
#define URX_TRANSFORM_H

#include <iosfwd>
#include <string>
#include <urx/v0_5/object.h>
#include <urx/v0_5/rotation.h>
#include <urx/v0_5/translation.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

/**
 * @brief The URX Transform class
 *  Defines an affine transformation in a 3D Cartesian system.
 *  A transform is composed by a rotation R = (rx, ry, rz)
 *  followed by a translation T = (tx, ty, tz).
 */
class Transform : public urx::Object {
  URX_TYPE_MACRO(Transform, urx::Object);

 public:
  Transform() = default;

  Transform(const urx::Rotation& rotation, const urx::Translation& translation) {
    this->setRotation(rotation);
    this->setTranslation(translation);
  }

  void printSelf(std::ostream& os, const std::string& indent) const override;

  const urx::Rotation& rotation() const { return m_rotation; }
  void setRotation(const urx::Rotation& rotation) { m_rotation = rotation; }

  const urx::Translation& translation() const { return m_translation; }
  void setTranslation(const urx::Translation& translation) { m_translation = translation; }

  bool operator==(const Transform& other) const {
    return ((m_rotation == other.m_rotation) && (m_translation == other.m_translation));
  }

  inline bool operator!=(const Transform& other) const { return !(*this == other); }

  Transform& operator=(const Transform& other) {
    if (&other == this) return *this;
    m_rotation = other.m_rotation;
    m_translation = other.m_translation;
    return *this;
  }

 private:
  // rotation in radians
  urx::Rotation m_rotation;

  // position in meters
  urx::Translation m_translation;
};

}  // namespace urx::v0_5

#endif  // URX_TRANSFORM_H