#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/element.h>
#include <urx/v0_3/element_geometry.h>
#include <urx/v0_3/impulse_response.h>

namespace urx::v0_3 {

struct Probe {
  Transform transform;
  double focal_length;
  std::vector<Element> elements;
  std::vector<std::shared_ptr<ElementGeometry>> element_geometries;
  std::vector<std::shared_ptr<ImpulseResponse>> impulse_responses;

  virtual ~Probe() = default;
};

}  // namespace urx::v0_3
