/*!
 * Copyright Moduleus 2019
 * \file uff/translation.h
 * \brief
 */

#ifndef UFF_TRANSLATION_H
#define UFF_TRANSLATION_H

// UFF
#include "uff/object.h"

namespace uff {

/**
 * @brief The UFF Translation class
 */
class Translation : public uff::Object {
  UFF_TYPE_MACRO(Translation, uff::Object);

 public:
  explicit Translation(MetadataType tx = 0.f, MetadataType ty = 0.f, MetadataType tz = 0.f) {
    setX(tx);
    setY(ty);
    setZ(tz);
  }

  void printSelf(std::ostream& os, std::string indent) const override;

  MetadataType x() const { return m_tx; }
  void setX(MetadataType x) { m_tx = x; }

  MetadataType y() const { return m_ty; }
  void setY(MetadataType y) { m_ty = y; }

  MetadataType z() const { return m_tz; }
  void setZ(MetadataType z) { m_tz = z; }

  bool operator==(const Translation& other) const {
    return ((m_tx == other.m_tx) && (m_ty == other.m_ty) && (m_tz == other.m_tz));
  }

  inline bool operator!=(const Translation& other) const { return !(*this == other); }

 private:
  MetadataType m_tx;
  MetadataType m_ty;
  MetadataType m_tz;
};

}  // namespace uff

#endif  // UFF_TRANSLATION_H