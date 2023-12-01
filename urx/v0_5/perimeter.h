/*!
 * Copyright Moduleus
 * \file urx/perimeter.h
 * \brief
 */

#ifndef URX_PERIMETER_H
#define URX_PERIMETER_H

#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/v0_5/object.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

/**
 * @brief The URX Perimeter class
 * TODO: replace std<<>> by Position
 * TODO: << operator for adding points to perimeter
 */
class Perimeter : public Object {
  URX_TYPE_MACRO(Perimeter, Object);

 public:
  Perimeter() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::vector<std::pair<MetadataType, MetadataType> >& points() { return m_points; }
  void setPoints(const std::vector<std::pair<MetadataType, MetadataType> >& points) {
    m_points = points;
  }

  bool operator==(const Perimeter& other) const { return (m_points == other.m_points); }

  inline bool operator!=(const Perimeter& other) const { return !(*this == other); }

 private:
  // Vector of pair of points (x, y). It is assumed that perimeter is always in the X-Y plane.
  std::vector<std::pair<MetadataType, MetadataType> > m_points;
};

}  // namespace urx::v0_5

#endif  // URX_PERIMETER_H