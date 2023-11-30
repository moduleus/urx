/*!
 * Copyright Moduleus
 * \file urx/element_geometry.h
 * \brief
 */

#ifndef URX_ELEMENT_GEOMETRY_H
#define URX_ELEMENT_GEOMETRY_H

#include <iosfwd>
#include <string>
#include <urx/v0_5/object.h>
#include <urx/v0_5/perimeter.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

/**
 * @brief Describes the geometry of an ultrasonic element.
 */
class ElementGeometry : public urx::Object {
  URX_TYPE_MACRO(ElementGeometry, urx::Object);

 public:
  ElementGeometry() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  urx::Perimeter& perimeter() { return m_perimeter; }
  void setPerimeter(const urx::Perimeter& perimeter) { m_perimeter = perimeter; }

  bool operator==(const ElementGeometry& other) const { return (m_perimeter == other.m_perimeter); }

  inline bool operator!=(const ElementGeometry& other) const { return !(*this == other); }

 private:
  // (Optional) List of positions defining the element perimeter
  urx::Perimeter m_perimeter;
};

}  // namespace urx::v0_5

#endif  // URX_ELEMENT_GEOMETRY_H