#pragma once

#include <uff/probe.h>
#include <uff/uff.h>
#include <uff/vector.h>
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace uff {

/**
 * @brief The UFF class describing a RCA (Row Column Address) probe
 */
struct RcaArray : public Probe {
  inline bool operator==(const RcaArray& other) const {
    return (Probe::operator==(other) && nb_elements == other.nb_elements && pitch == other.pitch &&
            element_width == other.element_width && element_height == other.element_height);
  }
  inline bool operator!=(const RcaArray& other) const { return !(*this == other); }

  /// Number of elements in the axis x and y
  Vector2D<uint32_t> nb_elements{0u, 0u};

  /// Distance between the acoustic center of adyacent elements along the axis x and y [m]
  Vector2D<double> pitch{0., 0.};

  /// (Optional) Element width for x and y axis [m]
  std::optional<Vector2D<double>> element_width = std::nullopt;

  /// (Optional) Element height for x and y axis [m]
  std::optional<Vector2D<double>> element_height = std::nullopt;
};

}  // namespace uff
