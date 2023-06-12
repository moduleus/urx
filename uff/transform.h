#pragma once

#include <uff/rotation.h>
#include <uff/translation.h>
#include <uff/uff.h>
#include <iosfwd>
#include <string>

namespace uff {

/**
 * @brief The UFF Transform class
 *  Defines an affine transformation in a 3D Cartesian system.
 *  A transform is composed by a rotation R = (rx, ry, rz)
 *  followed by a translation T = (tx, ty, tz).
 */
class Transform {
  // CTOR & DTOR
 public:
  Transform() = default;
  Transform(const uff::Rotation& rotation, const uff::Translation& translation) {
    this->setRotation(rotation);
    this->setTranslation(translation);
  }
  Transform(const Transform&) = default;
  Transform(Transform&&) = default;
  ~Transform() = default;

  // Operators
 public:
  Transform& operator=(const Transform& other) noexcept = default;
  Transform& operator=(Transform&& other) noexcept = default;
  inline bool operator==(const Transform& other) const {
    return ((_rotation == other._rotation) && (_translation == other._translation));
  }
  inline bool operator!=(const Transform& other) const { return !(*this == other); }

  // Accessors
 public:
  inline const uff::Rotation& rotation() const { return _rotation; }
  inline void setRotation(const uff::Rotation& rotation) { _rotation = rotation; }

  inline const uff::Translation& translation() const { return _translation; }
  inline void setTranslation(const uff::Translation& translation) { _translation = translation; }

  // Members
 private:
  // rotation in radians
  uff::Rotation _rotation;

  // position in meters
  uff::Translation _translation;
};

}  // namespace uff
