#pragma once

#include <array>

#include <uff/uff.h>

namespace uff {

/**
 * @brief Contains a location in space in Cartesian coordinates and SI units.
 */
template <typename T>
class Coordinates3D {
 public:
  // CTOR & DTOR
  explicit Coordinates3D(T x, T y, T z) : _data{x, y, z} {}
  Coordinates3D(const Coordinates3D&) = default;
  Coordinates3D(Coordinates3D&&) noexcept = default;
  ~Coordinates3D() = default;

  // Operators
  Coordinates3D& operator=(const Coordinates3D& other) noexcept = default;
  Coordinates3D& operator=(Coordinates3D&& other) noexcept = default;
  inline bool operator==(const Coordinates3D& other) const { return _data == other._data; }
  inline bool operator!=(const Coordinates3D& other) const { return !(*this == other); }

  // Accessors
  inline T x() const { return _data[0]; }
  inline void setX(T x) { _data[0] = x; }

  inline T y() const { return _data[1]; }
  inline void setY(T y) { _data[1] = y; }

  inline T z() const { return _data[2]; }
  inline void setZ(T z) { _data[2] = z; }

  // Members
 private:
  std::array<T, 3> _data{0., 0., 0.};
};

template <typename T>
class Coordinates2D {
 public:
  // CTOR & DTOR
  explicit Coordinates2D(T x, T y) : _data{x, y} {}
  Coordinates2D(const Coordinates2D&) = default;
  Coordinates2D(Coordinates2D&&) noexcept = default;
  ~Coordinates2D() = default;

  // Operators
  Coordinates2D& operator=(const Coordinates2D& other) noexcept = default;
  Coordinates2D& operator=(Coordinates2D&& other) noexcept = default;
  inline bool operator==(const Coordinates2D& other) const { return _data == other._data; }
  inline bool operator!=(const Coordinates2D& other) const { return !(*this == other); }

  // Accessors
  inline T x() const { return _data[0]; }
  inline void setX(T x) { _data[0] = x; }

  inline T y() const { return _data[1]; }
  inline void setY(T y) { _data[1] = y; }

  // Members
 private:
  std::array<T, 2> _data{0., 0.};
};

}  // namespace uff
