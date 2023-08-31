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
 * @brief The UFF RCA Array class describes a RCA (Row Column Address) probe
 */
struct RcaArray : public Probe {
  RcaArray() = delete;
  explicit RcaArray(Vector2D<uint32_t> nb_elements, const Vector2D<double>& pitch,
                    const std::optional<Vector2D<double>>& element_width = std::nullopt,
                    const std::optional<Vector2D<double>>& element_height = std::nullopt)
      : nb_elements(nb_elements),
        pitch(pitch),
        element_width(element_width),
        element_height(element_height) {
    updateElements();
  }
  RcaArray(const RcaArray&) = default;
  RcaArray(RcaArray&&) = default;
  ~RcaArray() = default;

  // Operators
  RcaArray& operator=(const RcaArray& other) noexcept = default;
  RcaArray& operator=(RcaArray&& other) noexcept = default;
  inline bool operator==(const RcaArray& other) const {
    return (Probe::operator==(other) && nb_elements == other.nb_elements && pitch == other.pitch &&
            element_width == other.element_width && element_height == other.element_height);
  }
  inline bool operator!=(const RcaArray& other) const { return !(*this == other); }

  // Update elements position
  void updateElements() {
    elements.resize(static_cast<size_t>(nb_elements.x) + nb_elements.y);

    const double xmin = -pitch.x * (nb_elements.x - 1.) / 2.;
    for (uint32_t i = 0; i < nb_elements.x; i++) {
      elements[i] =
          Element({Vector3D<double>{xmin + i * pitch.x, 0., 0.}, Vector3D<double>{0., 0., 0.}});
    }

    const double ymin = -pitch.y * (nb_elements.y - 1.) / 2.;
    for (uint32_t i = nb_elements.y; i < elements.size(); i++) {
      elements[i] = Element({Vector3D<double>{0., ymin + (i - nb_elements.y) * pitch.y, 0.},
                             Vector3D<double>{0., 0., 0.}});
    }
  }

  // Number of elements in the axis x and y
  Vector2D<uint32_t> nb_elements{0u, 0u};

  // Distance between the acoustic center of adyacent elements along the axis x and y [m]
  Vector2D<double> pitch{0., 0.};

  // (Optional) Element width for x and y axis [m]
  std::optional<Vector2D<double>> element_width = std::nullopt;

  // (Optional) Element height for x and y axis [m]
  std::optional<Vector2D<double>> element_height = std::nullopt;
};

}  // namespace uff
