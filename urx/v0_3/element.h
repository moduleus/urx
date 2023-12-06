#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/element_geometry.h>
#include <urx/v0_3/impulse_response.h>
#include <urx/v0_3/transform.h>

namespace urx::v0_3 {

struct Element {
  Transform transform;
  std::shared_ptr<ElementGeometry> element_geometry;
  std::shared_ptr<ImpulseResponse> impulse_response;
};

}  // namespace urx::v0_3
