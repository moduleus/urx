/*!
 * Copyright Moduleus
 * \file uff_element.h
 * \brief
 */

#ifndef UFF_ELEMENT_H
#define UFF_ELEMENT_H

// UFF
#include "uff_element_geometry.h"
#include "uff_impulse_response.h"
#include "uff_object.h"
#include "uff_transform.h"

// System
#include <limits>
#include <optional>

namespace uff {

/**
 * @brief UFF class to define an ultrasonic element
 */
class Element : public uff::Object {
  UFF_TYPE_MACRO(Element, uff::Object);

 public:
  Element() {}
  virtual ~Element() = default;

  void printSelf(std::ostream& os, std::string indent) const override;

  std::optional<FloatingType> x() const { return m_x; }
  std::optional<FloatingType> y() const { return m_y; }
  std::optional<FloatingType> z() const { return m_z; }

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wmaybe-uninitialized"
#endif
  void setX(std::optional<FloatingType> x) { m_x = x; }
  void setY(std::optional<FloatingType> y) { m_y = y; }
  void setZ(std::optional<FloatingType> z) { m_z = z; }
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
  void setXYZ(std::optional<FloatingType> x, std::optional<FloatingType> y,
              std::optional<FloatingType> z) {
    setX(x);
    setY(y);
    setZ(z);
  }

  bool operator==(const Element& other) const {
    return ((m_x == other.m_x) && (m_y == other.m_y) && (m_z == other.m_z));
  }

  inline bool operator!=(const Element& other) const { return !(*this == other); }

 private:
  std::optional<FloatingType> m_x = std::nullopt;
  std::optional<FloatingType> m_y = std::nullopt;
  std::optional<FloatingType> m_z = std::nullopt;
};

}  // namespace uff

#endif  // UFF_ELEMENT_H
