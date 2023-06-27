#pragma once

#include <uff/point.h>
#include <uff/uff.h>

#include <iosfwd>
#include <string>
#include <vector>

namespace uff {

/**
 * @brief Describes the geometry of an ultrasonic element.
 */
class ElementGeometry {
  // CTOR & DTOR
 public:
  ElementGeometry() = default;
  ElementGeometry(const ElementGeometry&) = default;
  ElementGeometry(ElementGeometry&&) = default;
  ~ElementGeometry() = default;

  // Operators
 public:
  ElementGeometry& operator=(const ElementGeometry& other) noexcept = default;
  ElementGeometry& operator=(ElementGeometry&& other) noexcept = default;
  bool operator==(const ElementGeometry& other) const { return (_perimeter == other._perimeter); }
  inline bool operator!=(const ElementGeometry& other) const { return !(*this == other); }

  // Accessors
 public:
  std::vector<uff::Point2D<MetadataType>>& perimeter() { return _perimeter; }
  void setPerimeter(const std::vector<uff::Point2D<MetadataType>>& perimeter) { _perimeter = perimeter; }

  // Members
 private:
  // (Optional) List of positions points (x,y) defining the element perimeter
  // It is assumed that perimeter is always in the X-Y plan
  std::vector<uff::Point2D<MetadataType>> _perimeter;
};

}  // namespace uff
