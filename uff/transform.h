#pragma once

#include <uff/point.h>
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
  Transform(const uff::Point3D<MetadataType>& rotation, const uff::Point3D<MetadataType>& translation) : _rotation(rotation), _translation(translation) {}
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
  inline const uff::Point3D<MetadataType>& rotation() const { return _rotation; }
  inline void setRotation(const uff::Point3D<MetadataType>& rotation) { _rotation = rotation; }

  inline const uff::Point3D<MetadataType>& translation() const { return _translation; }
  inline void setTranslation(const uff::Point3D<MetadataType>& translation) { _translation = translation; }

  // Members
 private:
  // Rotation around the 3 axis X: elevation, Y: azimut and Z: roll in radians
  uff::Point3D<MetadataType> _rotation;

  // translation in meters on 3 axis X, Y and Z
  uff::Point3D<MetadataType> _translation;
};

}  // namespace uff
