#pragma once

#include <memory>

#include <urx/detail/compare.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/transform.h>
#include <urx/urx.h>

namespace urx {

/**
 * @brief The URX class describing the ultrasonic element.
 */
struct Element {
  bool operator==(const Element& other) const = default;

  /// Location of the aperture center in the probe space reference
  Transform transform;

  /// Reference the corresponding element geometry in the probe the element belong to
  std::weak_ptr<ElementGeometry> element_geometry = std::weak_ptr<ElementGeometry>();

  /// Reference the corresponding impulse response in the probe the element belong to
  std::weak_ptr<ImpulseResponse> impulse_response = std::weak_ptr<ImpulseResponse>();
};

}  // namespace urx
