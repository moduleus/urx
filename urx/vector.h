#pragma once

#include <compare>

#include <urx/urx.h>

namespace urx {

template <typename T>
struct Vector3D {
  bool operator==(const Vector3D<T>& other) const = default;

  T x = 0.;
  T y = 0.;
  T z = 0.;
};

template <typename T>
struct Vector2D {
  bool operator==(const Vector2D<T>& other) const = default;

  T x = 0.;
  T y = 0.;
};

}  // namespace urx
