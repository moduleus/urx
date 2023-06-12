#pragma once

#include <uff/uff.h>
#include <iosfwd>
#include <string>

namespace uff {

/**
 * @brief The UFF Rotation class contains a rotation in space in spherical coordinates and SI units. 
 * The rotation is specified using Euler angles that are applied in the order ZYX.
 */
class Rotation {
  // CTOR & DTOR
 public:
  explicit Rotation(MetadataType rx = 0.f, MetadataType ry = 0.f, MetadataType rz = 0.f)
      : _rx(rx), _ry(ry), _rz(rz) {}
  Rotation(const Rotation&) = default;
  Rotation(Rotation&&) = default;
  ~Rotation() = default;

  // Operators
 public:
  Rotation& operator=(const Rotation& other) noexcept = default;
  Rotation& operator=(Rotation&& other) noexcept = default;
  bool operator==(const Rotation& other) const {
    return ((_rx == other._rx) && (_ry == other._ry) && (_rz == other._rz));
  }
  inline bool operator!=(const Rotation& other) const { return !(*this == other); }

  // Accessors
 public:
  inline MetadataType x() const { return _rx; }
  inline void setX(MetadataType x) { _rx = x; }

  inline MetadataType y() const { return _ry; }
  inline void setY(MetadataType y) { _ry = y; }

  inline MetadataType z() const { return _rz; }
  inline void setZ(MetadataType z) { _rz = z; }

  inline MetadataType elevation() const { return _rx; }
  inline MetadataType azimut() const { return _ry; }
  inline MetadataType roll() const { return _rz; }

  // Members
 private:
  // Rotation around the X-axis (elevation) in radians
  MetadataType _rx;

  // Rotation around the Y-axis (azimut) in radians
  MetadataType _ry;

  // Rotation around the Z-axis (roll) in radians
  MetadataType _rz;
};

}  // namespace uff
