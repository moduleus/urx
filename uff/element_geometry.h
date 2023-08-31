#pragma once

#include <uff/uff.h>
#include <uff/vector.h>

#include <iosfwd>
#include <string>
#include <vector>

namespace uff {

/**
 * @brief Describes the geometry of an ultrasonic element.
 */
struct ElementGeometry {
  auto operator<=>(const ElementGeometry& other) const = default;

  // List of positions points (x,y,z) defining the element perimeter
  std::vector<Vector3D<double>> perimeter;
};

}  // namespace uff
