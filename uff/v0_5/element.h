/*!
 * Copyright Moduleus
 * \file uff/element.h
 * \brief
 */

#ifndef UFF_ELEMENT_H
#define UFF_ELEMENT_H

#include <iosfwd>
#include <optional>
#include <string>

#include "uff/v0_5/object.h"
#include "uff/v0_5/uff.h"

namespace uff::v0_5 {

/**
 * @brief UFF class to define an ultrasonic element
 */
class Element : public uff::Object {
  UFF_TYPE_MACRO(Element, uff::Object);

 public:
  Element() = default;
  ~Element() override = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::optional<MetadataType> x() const { return m_x; }
  std::optional<MetadataType> y() const { return m_y; }
  std::optional<MetadataType> z() const { return m_z; }

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wmaybe-uninitialized"
#endif
  void setX(std::optional<MetadataType> x) { m_x = x; }
  void setY(std::optional<MetadataType> y) { m_y = y; }
  void setZ(std::optional<MetadataType> z) { m_z = z; }
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
  void setXYZ(std::optional<MetadataType> x, std::optional<MetadataType> y,
              std::optional<MetadataType> z) {
    setX(x);
    setY(y);
    setZ(z);
  }

  bool operator==(const Element& other) const {
    return ((m_x == other.m_x) && (m_y == other.m_y) && (m_z == other.m_z));
  }

  inline bool operator!=(const Element& other) const { return !(*this == other); }

 private:
  std::optional<MetadataType> m_x = std::nullopt;
  std::optional<MetadataType> m_y = std::nullopt;
  std::optional<MetadataType> m_z = std::nullopt;
};

}  // namespace uff::v0_5

#endif  // UFF_ELEMENT_H
