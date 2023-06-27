#pragma once

#include <uff/point.h>
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
 * @brief The UFF MatrixArray class describes a 2D matrix array of elements
 * Element[0] has the minimum x/y coordinates. Element[1] has the same y-coordinates as
 * element[0] but a higher x-coordinates.
 */
class MatrixArray : public uff::Probe {
  // CTOR & DTOR
 public:
  MatrixArray() = default;
  MatrixArray(const Point2D<uint32_t>& nb_elements, const Point2D<MetadataType>& pitch)
      : _nb_elements(nb_elements), _pitch(pitch) {
    updateElements();
  }
  MatrixArray(const MatrixArray&) = default;
  MatrixArray(MatrixArray&&) = default;
  virtual ~MatrixArray() override = default;

  // Operators
 public:
  MatrixArray& operator=(const MatrixArray& other) noexcept = default;
  MatrixArray& operator=(MatrixArray&& other) noexcept = default;
  inline bool operator==(const MatrixArray& other) const {
    return (Probe::operator==(other) && _nb_elements == other._nb_elements &&
            _pitch == other._pitch && _element_size == other._element_size);
  }
  inline bool operator!=(const MatrixArray& other) const { return !(*this == other); }

  // Accessors
 public:
  inline Point2D<uint32_t> nbElements() const { return _nb_elements; }
  inline void setNumberElements(Point2D<uint32_t> nb_elements) {
    _nb_elements = nb_elements;
    updateElements();
  }

  inline Point2D<MetadataType> pitch() const { return _pitch; }
  inline void setPitch(const Point2D<MetadataType>& pitch) {
    _pitch = pitch;
    updateElements();
  }

  inline void setElementSize(std::optional<Point2D<MetadataType>> element_size) {
    _element_size = element_size;
  }
  inline std::optional<Point2D<MetadataType>> elementSize() const { return _element_size; }

 private:
  // Update elements position
  void updateElements() override {
    _elements.resize(static_cast<size_t>(_nb_elements.x()) * _nb_elements.y());

    MetadataType xmin = -_pitch.x() * (_nb_elements.x() - 1.f) / 2.f;
    MetadataType ymin = -_pitch.y() * (_nb_elements.y() - 1.f) / 2.f;
    for (uint32_t i = 0; i < _nb_elements.y(); i++) {
      for (uint32_t j = 0; j < _nb_elements.x(); j++) {
        _elements[static_cast<size_t>(j) + static_cast<size_t>(i) * _nb_elements.y()] = {
            xmin + j * _pitch.x(), ymin + i * _pitch.y(), 0.f};
      }
    }
  }

  // Members
 protected:
  // Number of elements in the axis x and y
  Point2D<uint32_t> _nb_elements{0u, 0u};

  // Distance between the acoustic center of adjacent elements along the axis x and y [m]
  Point2D<MetadataType> _pitch{0.f, 0.f};

  // (Optional) Element size in the axis x and y [m]
  std::optional<Point2D<MetadataType>> _element_size = std::nullopt;
};

}  // namespace uff
