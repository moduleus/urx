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
 * @brief The UFF LinearArray class describes a linear array (1D)
 */
struct LinearArray : public Probe {
  LinearArray() = delete;
  explicit LinearArray(uint32_t number_elements, double pitch,
                       const std::optional<Vector2D<double>>& element_size = std::nullopt)
      : nb_elements(number_elements), pitch(pitch), element_size(element_size) {
    updateElements();
  }
  LinearArray(const LinearArray&) = default;
  LinearArray(LinearArray&&) = default;
  ~LinearArray() = default;

  // Operators
  LinearArray& operator=(const LinearArray& other) noexcept = default;
  LinearArray& operator=(LinearArray&& other) noexcept = default;
  inline bool operator==(const LinearArray& other) const {
    return (Probe::operator==(other) && nb_elements == other.nb_elements && pitch == other.pitch &&
            element_size == other.element_size);
  }
  inline bool operator!=(const LinearArray& other) const { return !(*this == other); }

  // Update elements position
  void updateElements() {
    elements.resize(nb_elements);
    for (uint32_t i = 0; i < nb_elements; ++i) {
      // element position
      double xmin = -pitch * static_cast<float>(nb_elements - 1) / 2.f;
      elements[i] =
          Element({Vector3D<double>{xmin + i * pitch, 0., 0.}, Vector3D<double>{0., 0., 0.}});
    }
  }

  // Number of elements in the array
  uint32_t nb_elements = 0;

  // Distance between the acoustic ceneter of adyacent elements [m]
  double pitch = 0;

  // (Optional) Element size in the axis x and y [m]
  std::optional<Vector2D<double>> element_size = std::nullopt;
};

}  // namespace uff
