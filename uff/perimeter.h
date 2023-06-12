#pragma once

#include <uff/uff.h>
#include <utility>
#include <vector>

namespace uff {

/**
 * @brief The UFF Perimeter class
 * TODO: replace std<<>> by uff::Position
 * TODO: << operator for adding points to perimeter
 */
class Perimeter {
  // CTOR & DTOR
 public:
  Perimeter() = default;
  Perimeter(const Perimeter&) = default;
  Perimeter(Perimeter&&) = default;
  ~Perimeter() = default;

  // Operators
 public:
  Perimeter& operator=(const Perimeter& other) noexcept = default;
  Perimeter& operator=(Perimeter&& other) noexcept = default;
  bool operator==(const Perimeter& other) const { return (m_points == other.m_points); }
  inline bool operator!=(const Perimeter& other) const { return !(*this == other); }

  // Accessors
 public:
  inline std::vector<std::pair<MetadataType, MetadataType> >& points() { return m_points; }
  inline void setPoints(const std::vector<std::pair<MetadataType, MetadataType> >& points) {
    m_points = points;
  }

  // Members
 private:
  // Vector of pair of points (x, y). It is assumed that perimeter is always in the X-Y plane.
  std::vector<std::pair<MetadataType, MetadataType> > m_points;
};

}  // namespace uff
