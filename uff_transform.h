/*!
 * Copyright Moduleus
 * \file uff_transform.h
 * \brief
 */

#ifndef UFF_TRANSFORM_H
#define UFF_TRANSFORM_H

// UFF
#include "uff_object.h"
#include "uff_rotation.h"
#include "uff_translation.h"

namespace uff {

/**
 * @brief The UFF Transform class
 *  Defines an affine transformation in a 3D Cartesian system.
 *  A transform is composed by a rotation R = (rx, ry, rz)
 *  followed by a translation T = (tx, ty, tz).
 */
class Transform : public uff::Object {
  UFF_TYPE_MACRO(Transform, uff::Object);

 public:
  Transform() {}

  Transform(const uff::Rotation& rotation,
            const uff::Translation& translation) {
    this->setRotation(rotation);
    this->setTranslation(translation);
  }

  void printSelf(std::ostream& os, std::string indent) const override;

  const uff::Rotation& rotation() const { return m_rotation; }
  void setRotation(const uff::Rotation& rotation) { m_rotation = rotation; }

  const uff::Translation& translation() const { return m_translation; }
  void setTranslation(const uff::Translation& translation) {
    m_translation = translation;
  }

  bool operator==(const Transform& other) const {
    return ((m_rotation == other.m_rotation) &&
            (m_translation == other.m_translation));
  }

  inline bool operator!=(const Transform& other) const {
    return !(*this == other);
  }

  Transform& operator=(const Transform& other) {
    m_rotation = other.m_rotation;
    m_translation = other.m_translation;
    return *this;
  }

 private:
  // rotation in radians
  uff::Rotation m_rotation;

  // position in meters
  uff::Translation m_translation;
};

}  // namespace uff

#endif  // UFF_TRANSFORM_H