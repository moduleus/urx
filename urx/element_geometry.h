#pragma once

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include <urx/urx.h>
#include <urx/vector.h>

namespace urx {

/**
 * @brief The URX class describing the geometry of an ultrasonic element.
 */
struct ElementGeometry {
  bool operator==(const ElementGeometry& other) const = default;

  /// List of positions points (x,y,z) defining the element perimeter
  std::vector<Vector3D<double>> perimeter;
};

}  // namespace urx
