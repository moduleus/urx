#pragma once

#include <uff/uff.h>
#include <uff/vector.h>
#include <iosfwd>
#include <string>

namespace uff {

/**
 * @brief The UFF class describing a transformation
 * 
 *  Defines an affine transformation in a 3D Cartesian system.
 *  A transform is composed by a rotation R = (rx, ry, rz)
 *  followed by a translation T = (tx, ty, tz).
 */
struct Transform {
  auto operator<=>(const Transform& other) const = default;

  /// Rotation around the 3 axis X: elevation, Y: azimut and Z: roll [rad]
  Vector3D<double> rotation;

  /// translation on 3 axis X, Y and Z [m]
  Vector3D<double> translation;
};

}  // namespace uff
