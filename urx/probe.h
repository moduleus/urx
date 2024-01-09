#pragma once

#include <cassert>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/transform.h>
#include <urx/urx.h>

namespace urx {

struct Probe {
  enum class ProbeType {
    LINEAR = 0,
    CURVILINEAR = 1,
    RCA = 2,
    MATRIX = 3,
    SPARSE = 4,
    UNDEFINED = -1
  };

  bool operator==(const Probe& other) const {
    return description == other.description && type == other.type && transform == other.transform &&
           element_geometries == other.element_geometries &&
           impulse_responses == other.impulse_responses && elements == other.elements;
  }

  bool operator!=(const Probe& other) const { return !operator==(other); }

  std::string description;

  ProbeType type = ProbeType::UNDEFINED;

  Transform transform;

  std::vector<std::shared_ptr<ElementGeometry>> element_geometries;
  std::vector<std::shared_ptr<ImpulseResponse>> impulse_responses;

  std::vector<Element> elements;
};

}  // namespace urx
