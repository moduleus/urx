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
 * @brief The UFF RCA Array class describes a RCA (Row Column Address) probe
 */
class RcaArray : public uff::Probe {
  // CTOR & DTOR
 public:
  explicit RcaArray(Point2D<uint32_t> nb_elements) : _nb_elements(nb_elements) {
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
    return (Probe::operator==(other) && _nb_elements == other._nb_elements &&
            _pitch == other._pitch && _element_width == other._element_width &&
            _element_height == other._element_height);
  }
  inline bool operator!=(const RcaArray& other) const { return !(*this == other); }

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

  inline std::optional<Point2D<MetadataType>> elementWidth() const { return _element_width; }
  inline void setElementWidth(std::optional<Point2D<MetadataType>> elementWidth) {
    _element_width = elementWidth;
  }

  inline std::optional<Point2D<MetadataType>> elementHeight() const { return _element_height; }
  inline void setElementHeightY(std::optional<Point2D<MetadataType>> elementHeight) {
    _element_height = elementHeight;
  }

 private:
  // Update elements position
  void updateElements() override {
    _elements.resize(static_cast<size_t>(_nb_elements.x()) + _nb_elements.y());

    MetadataType xmin = -_pitch.x() * (_nb_elements.x() - 1.f) / 2.f;
    for (uint32_t i = 0; i < _nb_elements.x(); i++) {
      _elements[i] = {xmin + i * _pitch.x(), 0.f, 0.f};
    }

    MetadataType ymin = -_pitch.y() * (_nb_elements.y() - 1.f) / 2.f;
    for (uint32_t i = _nb_elements.y(); i < _elements.size(); i++) {
      _elements[i] = {0.f, ymin + (i - _nb_elements.y()) * _pitch.y(), 0.f};
    }
  }

  // Members
 protected:
  // Number of elements in the axis x and y
  Point2D<uint32_t> _nb_elements{0u, 0u};

  // Distance between the acoustic center of adyacent elements along the axis x and y [m]
  Point2D<MetadataType> _pitch{0.f, 0.f};

  // (Optional) Element size in the x-axis [m]
  std::optional<Point2D<MetadataType>> _element_width = std::nullopt;

  // (Optional) Element size in the y-axis [m]
  std::optional<Point2D<MetadataType>> _element_height = std::nullopt;
};

}  // namespace uff
