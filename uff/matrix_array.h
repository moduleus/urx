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
  MatrixArray() = delete;
  MatrixArray(const Vector2D<uint32_t>& nb_elements, const Vector2D<double>& pitch,
              const std::optional<Vector2D<double>>& element_size = std::nullopt)
      : nb_elements(nb_elements), pitch(pitch), element_size(element_size) {
    updateElements();
  }
  MatrixArray(const MatrixArray&) = default;
  MatrixArray(MatrixArray&&) = default;
  ~MatrixArray() = default;

  // Operators
  MatrixArray& operator=(const MatrixArray& other) noexcept = default;
  MatrixArray& operator=(MatrixArray&& other) noexcept = default;
  inline bool operator==(const MatrixArray& other) const {
    return (Probe::operator==(other) && nb_elements == other.nb_elements && pitch == other.pitch &&
            element_size == other.element_size);
  }
  inline bool operator!=(const MatrixArray& other) const { return !(*this == other); }

  void updateElements() {
    elements.resize(static_cast<size_t>(nb_elements.x) * nb_elements.y);

    double xmin = -pitch.x * (nb_elements.x - 1.f) / 2.f;
    double ymin = -pitch.y * (nb_elements.y - 1.f) / 2.f;
    for (uint32_t i = 0; i < nb_elements.y; i++) {
      for (uint32_t j = 0; j < nb_elements.x; j++) {
        elements[static_cast<size_t>(j) + static_cast<size_t>(i) * nb_elements.y] = Element(
            {Vector3D<double>(xmin + j * pitch.x, ymin + i * pitch.y, 0.f), Vector3D<double>()});
      }
    }
  }

  // Number of elements in the axis x and y
  Vector2D<uint32_t> nb_elements{0u, 0u};

  // Distance between the acoustic center of adjacent elements along the axis x and y [m]
  Vector2D<double> pitch{0.f, 0.f};

  // (Optional) Element size in the axis x and y [m]
  std::optional<Vector2D<double>> element_size = std::nullopt;
};

}  // namespace uff
