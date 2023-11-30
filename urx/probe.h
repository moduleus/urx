#pragma once

#include <cassert>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <urx/detail/compare.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/transform.h>
#include <urx/urx.h>

namespace urx {

/**
 * @brief The URX class describing a generic ultrasound probe formed by a collection of elements
 */
struct Probe {
  enum class ProbeType {
    LINEAR = 0,
    CURVILINEAR = 1,
    RCA = 2,
    MATRIX = 3,
    SPARSE = 4,
    UNDEFINED = -1
  };

  bool operator==(const Probe& other) const = default;

  /// Description of the probe with serial ID
  /// This description aims to identify correctly the probe used during the acquisition
  std::string description;

  /// Type of the probe, used to determine element geometries
  ProbeType type = ProbeType::UNDEFINED;

  /// Location of the probe center in space reference
  Transform transform;

  /// List of all unique element geometries in the probe
  std::vector<std::shared_ptr<ElementGeometry>> element_geometries;

  /// List of all unique electromechanical impulse responses of the elements in the probe
  std::vector<std::shared_ptr<ImpulseResponse>> impulse_responses;

  /// List of all the elements in the probe
  std::vector<Element> elements;
};

}  // namespace urx
