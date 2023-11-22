#pragma once

#include <uff/detail/compare.h>
#include <uff/element_geometry.h>
#include <uff/impulse_response.h>
#include <uff/transform.h>
#include <uff/uff.h>

#include <memory>

namespace uff {

/**
 * @brief The UFF class describing the ultrasonic element.
 */
struct Element {
  bool operator==(const Element& other) const = default;

  /// Location of the aperture center in the probe space reference
  Transform transform;

  /// Reference the corresponfing element geometry in the probe the element belong to
  std::weak_ptr<ElementGeometry> element_geometry = std::weak_ptr<ElementGeometry>();

  /// Reference the corresponfing impulse responce in the probe the element belong to
  std::weak_ptr<ImpulseResponse> impluse_response = std::weak_ptr<ImpulseResponse>();
};

}  // namespace uff