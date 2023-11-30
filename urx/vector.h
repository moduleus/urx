#pragma once

#include <compare>

#include <urx/urx.h>

namespace urx {

/**
 * @brief The URX class containing a location in 3D Cartesian coordinate
 */
template <typename T>
struct Vector3D {
  bool operator==(const Vector3D<T>& other) const = default;

  /// x [m]
  T x = 0.;

  /// y [m]
  T y = 0.;

  /// z [m]
  T z = 0.;
};

/**
 * @brief The URX class containing a location in 2D Cartesian coordinate
 */
template <typename T>
struct Vector2D {
  bool operator==(const Vector2D<T>& other) const = default;

  /// x [m]
  T x = 0.;

  /// y [m]
  T y = 0.;
};

}  // namespace urx
