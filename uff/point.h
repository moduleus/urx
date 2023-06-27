#pragma once

#include <array>

#include <uff/uff.h>

namespace uff {

/**
 * @brief Contains a location in space in Cartesian coordinates and SI units.
 */
template <typename T>
class Point3D {
  // CTOR & DTOR
 public:
  Point3D(T x = 0.f, T y = 0.f, T z = 0.f)
      : _data{x, y, z} {}
  Point3D(const Point3D&) = default;
  Point3D(Point3D&&) = default;
  ~Point3D() = default;

  // Operators
 public:
  Point3D& operator=(const Point3D& other) noexcept = default;
  Point3D& operator=(Point3D&& other) noexcept = default;
  inline bool operator==(const Point3D& other) const { return _data == other._data; }
  inline bool operator!=(const Point3D& other) const { return !(*this == other); }

  // Accessors
 public:
  inline T x() const { return _data[0]; }
  inline void setX(T x) { _data[0] = x; }

  inline T y() const { return _data[1]; }
  inline void setY(T y) { _data[1] = y; }

  inline T z() const { return _data[2]; }
  inline void setZ(T z) { _data[2] = z; }

  // Members
 private:
  std::array<T, 3> _data;
};

template <typename T>
class Point2D {
  // CTOR & DTOR
 public:
  explicit Point2D(T x = 0.f, T y = 0.f)
      : _data{x, y} {}
  Point2D(const Point2D&) = default;
  Point2D(Point2D&&) = default;
  ~Point2D() = default;

  // Operators
 public:
  Point2D& operator=(const Point2D& other) noexcept = default;
  Point2D& operator=(Point2D&& other) noexcept = default;
  inline bool operator==(const Point2D& other) const { return _data == other._data; }
  inline bool operator!=(const Point2D& other) const { return !(*this == other); }

  // Accessors
 public:
  inline T x() const { return _data[0]; }
  inline void setX(T x) { _data[0] = x; }

  inline T y() const { return _data[1]; }
  inline void setY(T y) { _data[1] = y; }

  // Members
 private:
  std::array<T, 2> _data;
};

}  // namespace uff
