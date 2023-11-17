#pragma once

#include <compare>

#include <uff/uff.h>

namespace uff {

/**
 * @brief The UFF class containing a location in 3D Cartesian coordinate
 */
template <typename T>
struct Vector3D {
  explicit Vector3D(T x_ = 0., T y_ = 0., T z_ = 0.) : x(x_), y(y_), z(z_) {}

  // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
  auto operator<=>(const Vector3D<T>& other) const = default;

  /// x [m]
  T x = 0.;

  /// y [m]
  T y = 0.;

  /// z [m]
  T z = 0.;
};

/**
 * @brief The UFF class containing a location in 2D Cartesian coordinate
 */
template <typename T>
struct Vector2D {
  explicit Vector2D(T x_ = 0., T y_ = 0.) : x(x_), y(y_) {}

  // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
  auto operator<=>(const Vector2D<T>& other) const = default;

  /// x [m]
  T x = 0.;

  /// y [m]
  T y = 0.;
};

}  // namespace uff
