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
 * @brief The UFF MatrixArray class describes a 2D matrix array of elements
 * Element[0] has the minimum x/y coordinates. Element[1] has the same y-coordinates as
 * element[0] but a higher x-coordinates.
 */
struct MatrixArray : public Probe {
  inline bool operator==(const MatrixArray& other) const {
    return (Probe::operator==(other) && nb_elements == other.nb_elements && pitch == other.pitch &&
            element_size == other.element_size);
  }
  inline bool operator!=(const MatrixArray& other) const { return !(*this == other); }

  // Number of elements in the axis x and y
  Vector2D<uint32_t> nb_elements{0u, 0u};

  // Distance between the acoustic center of adjacent elements along the axis x and y [m]
  Vector2D<double> pitch{0.f, 0.f};

  // (Optional) Element size in the axis x and y [m]
  std::optional<Vector2D<double>> element_size = std::nullopt;
};

}  // namespace uff
