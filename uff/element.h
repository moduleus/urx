#pragma once

#include <uff/uff.h>
#include <iosfwd>
#include <optional>
#include <string>

namespace uff {

class Element {
  // CTOR & DTOR
 public:
  Element() = default;
  Element(const Element&) = default;
  Element(Element&&) = default;
  ~Element() = default;

  // Operators
 public:
  Element& operator=(const Element& other) noexcept = default;
  Element& operator=(Element&& other) noexcept = default;
  inline bool operator==(const Element& other) const {
    return ((_x == other._x) && (_y == other._y) && (_z == other._z));
  }
  inline bool operator!=(const Element& other) const { return !(*this == other); }

  // Accessors
 public:
  inline std::optional<MetadataType> x() const { return _x; }
  inline std::optional<MetadataType> y() const { return _y; }
  inline std::optional<MetadataType> z() const { return _z; }

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wmaybe-uninitialized"
#endif
  inline void setX(std::optional<MetadataType> x) { _x = x; }
  inline void setY(std::optional<MetadataType> y) { _y = y; }
  inline void setZ(std::optional<MetadataType> z) { _z = z; }
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

  // Members
 private:
  std::optional<MetadataType> _x = std::nullopt;
  std::optional<MetadataType> _y = std::nullopt;
  std::optional<MetadataType> _z = std::nullopt;
};

}  // namespace uff
