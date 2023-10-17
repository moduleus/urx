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
 * @brief The UFF class describing a probe containing a 2D matrix array of elements
 * 
 * First element from the matrix is at the minimum coordinates on both x and y axis
 * Following element are increasing coordinates on x axis first and then on y axis each time the x axis limit is reached
 */
struct MatrixArray : public Probe {
  inline bool operator==(const MatrixArray& other) const {
    return (Probe::operator==(other) && nb_elements == other.nb_elements && pitch == other.pitch &&
            element_size == other.element_size);
  }
  inline bool operator!=(const MatrixArray& other) const { return !(*this == other); }

  /// Number of elements in the axis x and y
  Vector2D<uint32_t> nb_elements = Vector2D<uint32_t>(0u, 0u);

  /// Distance between the acoustic center of adjacent elements along the axis x and y [m]
  Vector2D<double> pitch = Vector2D<double>(0., 0.);

  /// (Optional) Element size in the axis x and y [m]
  std::optional<Vector2D<double>> element_size = std::nullopt;
};

}  // namespace uff
