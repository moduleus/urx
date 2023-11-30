/*!
 * Copyright Moduleus
 * \file uff/element_geometry.h
 * \brief
 */

#ifndef UFF_ELEMENT_GEOMETRY_H
#define UFF_ELEMENT_GEOMETRY_H

#include <iosfwd>
#include <string>
#include "uff/v0_5/object.h"
#include "uff/v0_5/perimeter.h"
#include "uff/v0_5/uff.h"

namespace uff::v0_5 {

/**
 * @brief Describes the geometry of an ultrasonic element.
 */
class ElementGeometry : public uff::Object {
  UFF_TYPE_MACRO(ElementGeometry, uff::Object);

 public:
  ElementGeometry() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  uff::Perimeter& perimeter() { return m_perimeter; }
  void setPerimeter(const uff::Perimeter& perimeter) { m_perimeter = perimeter; }

  bool operator==(const ElementGeometry& other) const { return (m_perimeter == other.m_perimeter); }

  inline bool operator!=(const ElementGeometry& other) const { return !(*this == other); }

 private:
  // (Optional) List of positions defining the element perimeter
  uff::Perimeter m_perimeter;
};

}  // namespace uff::v0_5

#endif  // UFF_ELEMENT_GEOMETRY_H