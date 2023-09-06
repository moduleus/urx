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
 * @brief The UFF Probe class describes a generic ultrsound probe formed by a collection of elements
 */
struct Probe {
  bool operator==(const Probe& other) const {
    return ((description == other.description) && (transform == other.transform) &&
            (focal_length == other.focal_length) && (elements == other.elements) &&
            (element_geometries == other.element_geometries) &&
            (impulse_responses == other.impulse_responses));
  }
  inline bool operator!=(const Probe& other) const { return !(*this == other); }

  // Description of the probe with serial ID
  std::string description;

  // Attitude of the probe in 3D
  Transform transform;

  // List all the elements in the probe
  std::vector<Element> elements;

  // [optional] For probes with a focal lens, it describes the focal length in m [Az, Ele]
  std::optional<double> focal_length = std::nullopt;

  // [optional] List of all unique element geometries in the probe
  std::vector<std::shared_ptr<ElementGeometry>> element_geometries;

  // [optional] List of unique electromechanical impulse responses of the elements in the probe
  std::vector<std::shared_ptr<ImpulseResponse>> impulse_responses;
};

}  // namespace uff
