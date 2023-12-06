#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/position.h>

namespace urx::v0_3 {

struct ElementGeometry {
  std::vector<Position> positions;
};

}  // namespace urx::v0_3
