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
 * @brief The UFF RCA Array class describes a RCA (Row Column Address) probe
 */
class RcaArray : public uff::Probe {
  // CTOR & DTOR
 public:
  explicit RcaArray(uint32_t nb_elements_x, uint32_t numberElementsy)
      : _nb_elements_x(nb_elements_x), _nb_elements_y(numberElementsy) {
    updateElements();
  }
  RcaArray(const RcaArray&) = default;
  RcaArray(RcaArray&&) = default;
  virtual ~RcaArray() override = default;

  // Operators
 public:
  RcaArray& operator=(const RcaArray& other) noexcept = default;
  RcaArray& operator=(RcaArray&& other) noexcept = default;
  inline bool operator==(const RcaArray& other) const {
    return (Probe::operator==(other) && _nb_elements_x == other._nb_elements_x &&
            _nb_elements_y == other._nb_elements_y && _pitch_x == other._pitch_x &&
            _pitch_y == other._pitch_y && _element_width_x == other._element_width_x &&
            _element_width_y == other._element_width_y &&
            _element_height_x == other._element_height_x &&
            _element_height_y == other._element_height_y);
  }
  inline bool operator!=(const RcaArray& other) const { return !(*this == other); }

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

  inline MetadataType pitchY() const { return _pitch_x; }
  inline void setPitchY(const MetadataType& pitch_y) {
    _pitch_y = pitch_y;
    updateElements();
  }

  inline std::optional<MetadataType> elementWidthX() const { return _element_width_x; }
  inline void setElementWidthX(std::optional<MetadataType> elementWidthX) {
    _element_width_x = elementWidthX;
  }

  inline std::optional<MetadataType> elementWidthY() const { return _element_width_y; }
  inline void setElementWidthY(std::optional<MetadataType> elementWidthY) {
    _element_width_y = elementWidthY;
  }

  inline std::optional<MetadataType> elementHeightX() const { return _element_height_x; }
  inline void setElementHeightX(std::optional<MetadataType> elementHeightX) {
    _element_height_x = elementHeightX;
  }

  inline std::optional<MetadataType> elementHeightY() const { return _element_height_y; }
  inline void setElementHeightY(std::optional<MetadataType> elementHeightY) {
    _element_height_y = elementHeightY;
  }

 private:
  // Update elements position
  void updateElements() override {
    _elements.resize(static_cast<size_t>(_nb_elements_x) + _nb_elements_y);

    MetadataType xmin = -_pitch_x * (_nb_elements_x - 1.f) / 2.f;

    for (uint32_t i = 0; i < _nb_elements_x; i++) {
      uff::Element element;
      element.setX(xmin + i * _pitch_x);
      element.setY(0.f);
      element.setZ(0.f);
      _elements[i] = element;
    }

    MetadataType ymin = -_pitch_y * (_nb_elements_y - 1.f) / 2.f;
    for (uint32_t i = _nb_elements_x; i < _elements.size(); i++) {
      uff::Element element;
      element.setY(ymin + (i - _nb_elements_x) * _pitch_y);
      element.setX(0.f);
      element.setZ(0.f);
      _elements[i] = element;
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
  std::optional<MetadataType> _element_width_x = std::nullopt;

  // (Optional) Element size in the x-axis [m]
  std::optional<MetadataType> _element_width_y = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<MetadataType> _element_height_x = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<MetadataType> _element_height_y = std::nullopt;
};

}  // namespace uff
