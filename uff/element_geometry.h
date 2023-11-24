#pragma once

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include <uff/uff.h>
#include <uff/vector.h>

namespace uff {

/**
 * @brief The UFF class describing the geometry of an ultrasonic element.
 */
struct ElementGeometry {
  bool operator==(const ElementGeometry& other) const = default;

  /// List of positions points (x,y,z) defining the element perimeter
  std::vector<Vector3D<double>> perimeter;
};

}  // namespace uff
