#pragma once

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include <urx/urx.h>
#include <urx/vector.h>

namespace urx {

struct ElementGeometry {
  bool operator==(const ElementGeometry& other) const = default;

  std::vector<Vector3D<double>> perimeter;
};

}  // namespace urx
