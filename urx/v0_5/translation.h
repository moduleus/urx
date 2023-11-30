/*!
 * Copyright Moduleus 2019
 * \file urx/translation.h
 * \brief
 */

#ifndef URX_TRANSLATION_H
#define URX_TRANSLATION_H

#include <iosfwd>
#include <string>

#include <urx/v0_5/object.h>
#include <urx/v0_5/urx.h>

namespace urx::v0_5 {

/**
 * @brief The URX Translation class
 */
class Translation : public Object {
  URX_TYPE_MACRO(Translation, Object);

 public:
  explicit Translation(MetadataType tx = 0.f, MetadataType ty = 0.f, MetadataType tz = 0.f) {
    setX(tx);
    setY(ty);
    setZ(tz);
  }

  void printSelf(std::ostream& os, const std::string& indent) const override;

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

}  // namespace urx::v0_5

#endif  // URX_TRANSLATION_H