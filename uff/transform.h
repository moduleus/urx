#pragma once

#include <uff/coordinates.h>
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
 public:
  // CTOR & DTOR
  Transform() = default;
  Transform(const Coordinates3D<MetadataType>& rotation,
            const Coordinates3D<MetadataType>& translation)
      : _rotation(rotation), _translation(translation) {}
  Transform(const Transform&) = default;
  Transform(Transform&&) noexcept = default;
  ~Transform() = default;

  // Operators
  Transform& operator=(const Transform& other) noexcept = default;
  Transform& operator=(Transform&& other) noexcept = default;
  inline bool operator==(const Transform& other) const {
    return ((_rotation == other._rotation) && (_translation == other._translation));
  }
  inline bool operator!=(const Transform& other) const { return !(*this == other); }

  // Accessors
  inline const Coordinates3D<MetadataType>& rotation() const { return _rotation; }
  inline void setRotation(const Coordinates3D<MetadataType>& rotation) { _rotation = rotation; }

  inline const Coordinates3D<MetadataType>& translation() const { return _translation; }
  inline void setTranslation(const Coordinates3D<MetadataType>& translation) {
    _translation = translation;
  }

  // Members
 private:
  // Rotation around the 3 axis X: elevation, Y: azimut and Z: roll in radians
  Coordinates3D<MetadataType> _rotation = Coordinates3D<MetadataType>(0, 0, 0);

  // translation in meters on 3 axis X, Y and Z
  Coordinates3D<MetadataType> _translation = Coordinates3D<MetadataType>(0, 0, 0);
  ;
};

}  // namespace uff
