#pragma once

#include <uff/probe.h>
#include <uff/uff.h>
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>
namespace uff {

/**
 * @brief The UFF class describing a linear array probe (1D)
 */
struct LinearArray : public Probe {
  inline bool operator==(const LinearArray& other) const {
    return (Probe::operator==(other) && nb_elements == other.nb_elements && pitch == other.pitch &&
            element_size == other.element_size);
  }
  inline bool operator!=(const LinearArray& other) const { return !(*this == other); }

  /// Number of elements in the array
  uint32_t nb_elements = 0;

  /// Distance between the acoustic ceneter of adyacent elements [m]
  double pitch = 0;

  /// (Optional) Element size in the axis x and y [m]
  std::optional<Vector2D<double>> element_size = std::nullopt;
};

}  // namespace uff
