#pragma once

#include <uff/uff.h>

namespace uff {

class Translation {
  // CTOR & DTOR
 public:
  explicit Translation(MetadataType tx = 0.f, MetadataType ty = 0.f, MetadataType tz = 0.f)
      : _tx(tx), _ty(ty), _tz(tz) {}
  Translation(const Translation&) = default;
  Translation(Translation&&) = default;
  ~Translation() = default;

  // Operators
 public:
  Translation& operator=(const Translation& other) noexcept = default;
  Translation& operator=(Translation&& other) noexcept = default;
  bool operator==(const Translation& other) const {
    return ((_tx == other._tx) && (_ty == other._ty) && (_tz == other._tz));
  }
  inline bool operator!=(const Translation& other) const { return !(*this == other); }

  // Accessors
 public:
  inline MetadataType x() const { return _tx; }
  inline void setX(MetadataType x) { _tx = x; }

  inline MetadataType y() const { return _ty; }
  inline void setY(MetadataType y) { _ty = y; }

  inline MetadataType z() const { return _tz; }
  inline void setZ(MetadataType z) { _tz = z; }

  // Members
 private:
  MetadataType _tx;
  MetadataType _ty;
  MetadataType _tz;
};

}  // namespace uff
