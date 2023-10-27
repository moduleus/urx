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
           (element_geometry == other.element_geometry) &&
           (impluse_response == other.impluse_response);
  }
  inline bool operator!=(const Element& other) const { return !(*this == other); }

  /// Location of the aperture center in the probe space reference
  Transform transform;

  /// Reference the corresponfing element geometry in the probe the element belong to
  std::shared_ptr<ElementGeometry> element_geometry;

  /// Reference the corresponfing impulse responce in the probe the element belong to
  std::shared_ptr<ImpulseResponse> impluse_response;
};

}  // namespace uff