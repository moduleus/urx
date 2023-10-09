#pragma once

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
  inline bool operator==(const Element& other) const {
    return (transform == other.transform) &&
           (element_geometry.expired() == other.element_geometry.expired()) &&
           (element_geometry.expired() ||
            *(element_geometry.lock()) == *(other.element_geometry.lock())) &&
           (impluse_response.expired() == other.impluse_response.expired()) &&
           (impluse_response.expired() ||
            *(impluse_response.lock()) == *(other.impluse_response.lock()));
  }
  inline bool operator!=(const Element& other) const { return !(*this == other); }

  /// Location of the element center in space reference
  Transform transform;

  /// Reference the corresponfing element geometry in the probe the element belong to
  std::weak_ptr<ElementGeometry> element_geometry = std::weak_ptr<ElementGeometry>();

  /// Reference the corresponfing impulse responce in the probe the element belong to
  std::weak_ptr<ImpulseResponse> impluse_response = std::weak_ptr<ImpulseResponse>();
};

}  // namespace uff