#pragma once

#include <compare>

#include <uff/uff.h>

namespace uff {

/**
 * @brief Contains a location in space in Cartesian coordinates and SI units.
 */
template <typename T>
struct Vector3D {
  explicit Vector3D(T x, T y, T z) : x(x), y(y), z(z) {}

  auto operator<=>(const Vector3D<T>& other) const = default;

  T x = 0.;
  T y = 0.;
  T z = 0.;
};

template <typename T>
struct Vector2D {
  explicit Vector2D(T x, T y) : x(x), y(y) {}

  auto operator<=>(const Vector2D<T>& other) const = default;

  T x = 0.;
  T y = 0.;
};

}  // namespace uff
