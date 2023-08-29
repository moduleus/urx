#pragma once

#include <compare>

#include <uff/uff.h>

namespace uff {

/**
 * @brief Contains a location in space in Cartesian coordinates and SI units.
 */
template <typename T>
struct Vector3D {
  explicit Vector3D(T x, T y, T z) : _x(x), _y(y), _z(z) {}

  auto operator<=>(const Vector3D<T>& other) const = default;

  T _x = 0.;
  T _y = 0.;
  T _z = 0.;
};

template <typename T>
struct Vector2D {
  explicit Vector2D(T x, T y) : _x(x), _y(y) {}

  auto operator<=>(const Vector2D<T>& other) const = default;

  T _x = 0.;
  T _y = 0.;
};

}  // namespace uff
