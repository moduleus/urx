#pragma once

#include <uff/perimeter.h>
#include <uff/uff.h>
#include <iosfwd>
#include <string>

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
  uff::Perimeter& perimeter() { return _perimeter; }
  void setPerimeter(const uff::Perimeter& perimeter) { _perimeter = perimeter; }

  // Members
 private:
  // (Optional) List of positions defining the element perimeter
  uff::Perimeter _perimeter;
};

}  // namespace uff
