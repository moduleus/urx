/*!
 * Copyright Moduleus 2019
 * \file uff_translation.h
 * \brief
 */

#ifndef UFF_TRANSLATION_H
#define UFF_TRANSLATION_H

// UFF
#include "uff_object.h"

namespace uff {

/**
 * @brief The UFF Translation class
 */
class Translation : public uff::Object {
  UFF_TYPE_MACRO(Translation, uff::Object);

 public:
  explicit Translation(FloatingType tx = 0.f, FloatingType ty = 0.f,
                       FloatingType tz = 0.f) {
    setX(tx);
    setY(ty);
    setZ(tz);
  }

  void printSelf(std::ostream& os, std::string indent) const override;

  FloatingType x() const { return m_tx; }
  void setX(FloatingType x) { m_tx = x; }

  FloatingType y() const { return m_ty; }
  void setY(FloatingType y) { m_ty = y; }

  FloatingType z() const { return m_tz; }
  void setZ(FloatingType z) { m_tz = z; }

  bool operator==(const Translation& other) const {
    return ((m_tx == other.m_tx) && (m_ty == other.m_ty) &&
            (m_tz == other.m_tz));
  }

  inline bool operator!=(const Translation& other) const {
    return !(*this == other);
  }

 private:
  FloatingType m_tx;
  FloatingType m_ty;
  FloatingType m_tz;
};

}  // namespace uff

#endif  // UFF_TRANSLATION_H