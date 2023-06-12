#pragma once

#include <uff.h>

namespace uff {

/**
 * @brief Contains a location in space in Cartesian coordinates and SI units.
 */
class Position {
  // CTOR & DTOR
 public:
  explicit Position(MetadataType x = 0.f, MetadataType y = 0.f, MetadataType z = 0.f)
      : _x(x), _y(y), _z(z) {}
  Position(const Position&) = default;
  Position(Position&&) = default;
  ~Position() override = default;

  // Operators
 public:
  Position& operator=(const Position& other) noexcept = default;
  Position& operator=(Position&& other) noexcept = default;
  inline bool operator==(const Position& other) const {
    return (_x == other._x && _y == other._y && _z == other._z);
  }
  inline bool operator!=(const Position& other) const { return !(*this == other); }

  // Accessors
 public:
  inline MetadataType x() const { return _x; }
  inline void setX(MetadataType x) { _x = x; }

  inline MetadataType y() const { return _y; }
  inline void setY(MetadataType y) { _x = y; }

  inline MetadataType z() const { return _z; }
  inline void setZ(MetadataType z) { _z = z; }

  // Members
 private:
  MetadataType _x;
  MetadataType _y;
  MetadataType _z;
};

}  // namespace uff
