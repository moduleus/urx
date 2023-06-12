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
 * @brief The UFF LinearArray class describes a linear array (1D)
 */
class LinearArray : public uff::Probe {
  // CTOR & DTOR
 public:
  explicit LinearArray(uint32_t numberElements) : _nb_elements(numberElements) { updateElements(); }
  LinearArray(const LinearArray&) = default;
  LinearArray(LinearArray&&) = default;
  virtual ~LinearArray() override = default;

  // Operators
 public:
  LinearArray& operator=(const LinearArray& other) noexcept = default;
  LinearArray& operator=(LinearArray&& other) noexcept = default;
  inline bool operator==(const LinearArray& other) const {
    return (Probe::operator==(other) && _nb_elements == other._nb_elements &&
            _pitch == other._pitch && _element_size == other._element_size);
  }
  inline bool operator!=(const LinearArray& other) const { return !(*this == other); }

  // Accessors
 public:
  inline uint32_t numberElements() const { return _nb_elements; }
  inline void setNumberElements(uint32_t numberElements) {
    _nb_elements = numberElements;
    updateElements();
  }

  inline MetadataType pitch() const { return _pitch; }
  inline void setPitch(const MetadataType& pitch) {
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
    _elements.resize(_nb_elements);
    for (uint32_t i = 0; i < _nb_elements; ++i) {
      // element position
      uff::Element element;
      MetadataType xmin = -_pitch * static_cast<float>(_nb_elements - 1) / 2.f;
      element.setX(xmin + i * _pitch);
      element.setY(0.f);
      element.setZ(0.f);
      _elements[i] = element;
    }
  }

  // Members
 protected:
  // Number of elements in the array
  uint32_t _nb_elements = 0;

  // Distance between the acoustic ceneter of adyacent elements [m]
  MetadataType _pitch = 0;

  // (Optional) Element size in the axis x and y [m]
  std::optional<Point2D<MetadataType>> _element_size = std::nullopt;

};

}  // namespace uff
