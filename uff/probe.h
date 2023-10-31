#pragma once

#include <uff/element.h>
#include <uff/element_geometry.h>
#include <uff/impulse_response.h>
#include <uff/transform.h>
#include <uff/uff.h>
#include <cassert>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
namespace uff {

/**
 * @brief The UFF class describing a generic ultrsound probe formed by a collection of elements
 */
struct Probe {
  bool operator==(const Probe& other) const {
    return ((description == other.description) && (transform == other.transform) &&
            (focal_length == other.focal_length) && (elements == other.elements) &&
            (element_geometries == other.element_geometries) &&
            (impulse_responses == other.impulse_responses));
  }
  inline bool operator!=(const Probe& other) const { return !(*this == other); }

  /// Description of the probe with serial ID
  /// This description aims to identify correctly the probe used during the acquisition
  std::string description = "";

  /// Location of the probe center in space reference
  Transform transform;

  /// List of all the elements in the probe
  std::vector<Element> elements;

  /// (Optional) For probes with a focal length only.
  /// Description of the focal length (Az, Ele) [m]
  std::optional<double> focal_length = std::nullopt;

  /// (Optional) List of all unique element geometries in the probe
  std::vector<ElementGeometry> element_geometries;

  /// (Optional) List of all unique electromechanical impulse responses of the elements in the probe
  std::vector<ImpulseResponse> impulse_responses;
};

}  // namespace uff
