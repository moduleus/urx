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
class MatrixArray : public Probe {
 public:
  // CTOR & DTOR
  MatrixArray() = delete;
  MatrixArray(const Vector2D<uint32_t>& nb_elements, const Vector2D<double>& pitch,
              const std::optional<Vector2D<double>>& element_size = std::nullopt)
      : _nb_elements(nb_elements), _pitch(pitch), _element_size(element_size) {
    updateElements();
  }
  MatrixArray(const MatrixArray&) = default;
  MatrixArray(MatrixArray&&) = default;
  ~MatrixArray() override = default;

  // Operators
  MatrixArray& operator=(const MatrixArray& other) noexcept = default;
  MatrixArray& operator=(MatrixArray&& other) noexcept = default;
  inline bool operator==(const MatrixArray& other) const {
    return (Probe::operator==(other) && _nb_elements == other._nb_elements &&
            _pitch == other._pitch && _element_size == other._element_size);
  }
  inline bool operator!=(const MatrixArray& other) const { return !(*this == other); }

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

  inline void setElementSize(std::optional<Vector2D<double>> element_size) {
    _element_size = element_size;
  }
  inline const std::optional<Vector2D<double>>& elementSize() const { return _element_size; }

 private:
  // Update elements position
  void updateElements() {
    _elements.resize(static_cast<size_t>(_nb_elements.x()) * _nb_elements.y());

    double xmin = -_pitch.x() * (_nb_elements.x() - 1.f) / 2.f;
    double ymin = -_pitch.y() * (_nb_elements.y() - 1.f) / 2.f;
    for (uint32_t i = 0; i < _nb_elements.y(); i++) {
      for (uint32_t j = 0; j < _nb_elements.x(); j++) {
        _elements[static_cast<size_t>(j) + static_cast<size_t>(i) * _nb_elements.y()] =
            Element({Vector3D<double>{xmin + j * _pitch.x(), ymin + i * _pitch.y(), 0.f},
                     Vector3D<double>{}});
      }
    }
  }

  // Members
 protected:
  // Number of elements in the axis x and y
  Vector2D<uint32_t> _nb_elements{0u, 0u};

  // Distance between the acoustic center of adjacent elements along the axis x and y [m]
  Vector2D<double> _pitch{0.f, 0.f};

  // (Optional) Element size in the axis x and y [m]
  std::optional<Vector2D<double>> _element_size = std::nullopt;
};

}  // namespace uff
