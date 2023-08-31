#pragma once

#include <uff/uff.h>
#include <uff/vector.h>
#include <iosfwd>
#include <string>

namespace uff {

/**
 * @brief The UFF Transform class
 *  Defines an affine transformation in a 3D Cartesian system.
 *  A transform is composed by a rotation R = (rx, ry, rz)
 *  followed by a translation T = (tx, ty, tz).
 */
struct Transform {
  auto operator<=>(const Transform& other) const = default;

  // Rotation around the 3 axis X: elevation, Y: azimut and Z: roll in radians
  Vector3D<double> rotation = Vector3D<double>(0., 0., 0.);

  // translation in meters on 3 axis X, Y and Z
  Vector3D<double> translation = Vector3D<double>(0., 0., 0.);
};

}  // namespace uff
