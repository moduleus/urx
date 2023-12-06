#pragma once

#include <memory>

#include <urx/detail/compare.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/transform.h>
#include <urx/urx.h>

namespace urx {

struct Element {
  bool operator==(const Element& other) const = default;

  Transform transform;

  std::weak_ptr<ElementGeometry> element_geometry = std::weak_ptr<ElementGeometry>();
  std::weak_ptr<ImpulseResponse> impulse_response = std::weak_ptr<ImpulseResponse>();
};

}  // namespace urx
