#pragma once

#include <uff/element.h>
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
  MatrixArray(const uint32_t& _nb_elements_x, const uint32_t& _nb_elements_y,
              const MetadataType& pitch_x, const MetadataType& pitch_y)
      : _nb_elements_x(_nb_elements_x),
        _nb_elements_y(_nb_elements_y),
        _pitch_x(pitch_x),
        _pitch_y(pitch_y) {
    updateElements();
  }
  MatrixArray(const RcaArray&) = default;
  MatrixArray(MatrixArray&&) = default;
  virtual ~MatrixArray() override = default;

  // Operators
 public:
  MatrixArray& operator=(const MatrixArray& other) noexcept = default;
  MatrixArray& operator=(MatrixArray&& other) noexcept = default;
  inline bool operator==(const MatrixArray& other) const {
    return (Probe::operator==(other) && _nb_elements_x == other._nb_elements_x &&
            _nb_elements_y == other._nb_elements_y && _pitch_x == other._pitch_x &&
            _pitch_y == other._pitch_y && _element_width_x == other._element_width_x &&
            _element_width_y == other._element_width_y &&
            _element_height_x == other._element_height_x &&
            _element_height_y == other._element_height_y);
  }
  inline bool operator!=(const MatrixArray& other) const { return !(*this == other); }

  // Accessors
 public:
  inline uint32_t nbElementsX() const { return _nb_elements_x; }
  inline void setNumberElementsX(uint32_t nb_elements_x) {
    _nb_elements_x = nb_elements_x;
    updateElements();
  }

  inline uint32_t nbElementsY() const { return _nb_elements_y; }
  inline void setNumberElementsY(uint32_t nb_elements_y) {
    _nb_elements_y = nb_elements_y;
    updateElements();
  }

  inline MetadataType pitchX() const { return _pitch_x; }
  inline void setPitchX(const MetadataType& pitch_x) {
    _pitch_x = pitch_x;
    updateElements();
  }

  inline MetadataType pitchY() const { return _pitch_y; }
  inline void setPitchY(const MetadataType& pitch_y) {
    _pitch_y = pitch_y;
    updateElements();
  }

  inline std::optional<MetadataType> element_width() { return _element_width; }
  inline void setelement_width(std::optional<MetadataType> element_width) {
    _element_width = element_width;
  }

  inline std::optional<MetadataType> element_height() { return _element_height; }
  inline void setelement_height(std::optional<MetadataType> element_height) {
    _element_height = element_height;
  }

 private:
  // Update elements position
  void updateElements() override {
    _elements.resize(static_cast<size_t>(_nb_elements_x) * _nb_elements_y);

    MetadataType xmin = -_pitch_x * (_nb_elements_x - 1.f) / 2.f;
    MetadataType ymin = -_pitch_y * (_nb_elements_y - 1.f) / 2.f;
    for (uint32_t i = 0; i < _nb_elements_y; i++) {
      for (uint32_t j = 0; j < _nb_elements_x; j++) {
        uff::Element element;
        element.setX(xmin + j * _pitch_x);
        element.setY(ymin + i * _pitch_y);
        element.setZ(0.f);
        _elements[static_cast<size_t>(j) + static_cast<size_t>(i) * _nb_elements_y] = element;
      }
    }
  }

  // Members
 protected:
  // Number of elements in the x-axis
  uint32_t _nb_elements_x = 0;

  // Number of elements in the y-axis
  uint32_t _nb_elements_y = 0;

  // Distance between the acoustic center of adyacent elements along the x-axis [m]
  MetadataType _pitch_x = 0;

  // Distance between the acoustic center of adyacent elements along the y-axis [m]
  MetadataType _pitch_y = 0;

  // (Optional) Element size in the x-axis [m]
  std::optional<MetadataType> _element_width = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<MetadataType> _element_height = std::nullopt;
};

}  // namespace uff
