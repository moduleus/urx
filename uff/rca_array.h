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
class RcaArray : public Probe {
 public:
  // CTOR & DTOR
  RcaArray() = delete;
  explicit RcaArray(Vector2D<uint32_t> nb_elements, const Vector2D<double>& pitch,
                    const std::optional<Vector2D<double>>& element_width = std::nullopt,
                    const std::optional<Vector2D<double>>& element_height = std::nullopt)
      : _nb_elements(nb_elements),
        _pitch(pitch),
        _element_width(element_width),
        _element_height(element_height) {
    updateElements();
  }
  RcaArray(const RcaArray&) = default;
  RcaArray(RcaArray&&) = default;
  ~RcaArray() override = default;

  // Operators
  RcaArray& operator=(const RcaArray& other) noexcept = default;
  RcaArray& operator=(RcaArray&& other) noexcept = default;
  inline bool operator==(const RcaArray& other) const {
    return (Probe::operator==(other) && _nb_elements == other._nb_elements &&
            _pitch == other._pitch && _element_width == other._element_width &&
            _element_height == other._element_height);
  }
  inline bool operator!=(const RcaArray& other) const { return !(*this == other); }

  // Accessors
  inline Vector2D<uint32_t> nbElements() const { return _nb_elements; }
  inline void setNumberElements(Vector2D<uint32_t> nb_elements) {
    _nb_elements = nb_elements;
    updateElements();
  }

  inline Vector2D<double> pitch() const { return _pitch; }
  inline void setPitch(const Vector2D<double>& pitch) {
    _pitch = pitch;
    updateElements();
  }

  inline std::optional<Vector2D<double>> elementWidth() const { return _element_width; }
  inline void setElementWidth(std::optional<Vector2D<double>> elementWidth) {
    _element_width = elementWidth;
  }

  inline std::optional<Vector2D<double>> elementHeight() const { return _element_height; }
  inline void setElementHeightY(std::optional<Vector2D<double>> elementHeight) {
    _element_height = elementHeight;
  }

 private:
  // Update elements position
  void updateElements() {
    _elements.resize(static_cast<size_t>(_nb_elements.x()) + _nb_elements.y());

    const double xmin = -_pitch.x() * (_nb_elements.x() - 1.) / 2.;
    for (uint32_t i = 0; i < _nb_elements.x(); i++) {
      _elements[i] =
          Element({Vector3D<double>{xmin + i * _pitch.x(), 0., 0.}, Vector3D<double>{0., 0., 0.}});
    }

    const double ymin = -_pitch.y() * (_nb_elements.y() - 1.) / 2.;
    for (uint32_t i = _nb_elements.y(); i < _elements.size(); i++) {
      _elements[i] = Element({Vector3D<double>{0., ymin + (i - _nb_elements.y()) * _pitch.y(), 0.},
                              Vector3D<double>{0., 0., 0.}});
    }
  }

  // Members
 protected:
  // Number of elements in the axis x and y
  Vector2D<uint32_t> _nb_elements{0u, 0u};

  // Distance between the acoustic center of adyacent elements along the axis x and y [m]
  Vector2D<double> _pitch{0., 0.};

  // (Optional) Element width for x and y axis [m]
  std::optional<Vector2D<double>> _element_width = std::nullopt;

  // (Optional) Element height for x and y axis [m]
  std::optional<Vector2D<double>> _element_height = std::nullopt;
};

}  // namespace uff
