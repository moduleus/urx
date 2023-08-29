#pragma once

#include <uff/uff.h>
#include <uff/vector.h>

#include <iosfwd>
#include <string>
#include <vector>

namespace uff {

/**
 * @brief Describes the geometry of an ultrasonic element.
 */
class ElementGeometry {
 public:
  // CTOR & DTOR
  ElementGeometry() = delete;
  explicit ElementGeometry(std::vector<Vector3D<double>> perimeter)
      : _perimeter(std::move(perimeter)) {}
  ElementGeometry(const ElementGeometry&) = default;
  ElementGeometry(ElementGeometry&&) = default;
  ~ElementGeometry() = default;

  // Operators
  ElementGeometry& operator=(const ElementGeometry& other) noexcept = default;
  ElementGeometry& operator=(ElementGeometry&& other) noexcept = default;
  bool operator==(const ElementGeometry& other) const { return (_perimeter == other._perimeter); }
  inline bool operator!=(const ElementGeometry& other) const { return !(*this == other); }

  // Accessors
  std::vector<Vector3D<double>>& perimeter() { return _perimeter; }
  void setPerimeter(const std::vector<Vector3D<double>>& perimeter) { _perimeter = perimeter; }

  // Members
 private:
  // List of positions points (x,y,z) defining the element perimeter
  std::vector<Vector3D<double>> _perimeter;
};

}  // namespace uff
