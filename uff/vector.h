#pragma once

#include <array>

#include <uff/uff.h>

namespace uff {

/**
 * @brief Contains a location in space in Cartesian coordinates and SI units.
 */
template <typename T>
class Vector3D {
 public:
  // CTOR & DTOR
  explicit Vector3D(T x, T y, T z) : _data{x, y, z} {}
  Vector3D(const Vector3D&) = default;
  Vector3D(Vector3D&&) noexcept = default;
  ~Vector3D() = default;

  // Operators
  Vector3D& operator=(const Vector3D& other) noexcept = default;
  Vector3D& operator=(Vector3D&& other) noexcept = default;
  inline bool operator==(const Vector3D& other) const { return _data == other._data; }
  inline bool operator!=(const Vector3D& other) const { return !(*this == other); }

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
class Vector2D {
 public:
  // CTOR & DTOR
  explicit Vector2D(T x, T y) : _data{x, y} {}
  Vector2D(const Vector2D&) = default;
  Vector2D(Vector2D&&) noexcept = default;
  ~Vector2D() = default;

  // Operators
  Vector2D& operator=(const Vector2D& other) noexcept = default;
  Vector2D& operator=(Vector2D&& other) noexcept = default;
  inline bool operator==(const Vector2D& other) const { return _data == other._data; }
  inline bool operator!=(const Vector2D& other) const { return !(*this == other); }

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
