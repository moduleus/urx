/*!
 * Copyright Moduleus
 * \file urx/version.h
 * \brief
 */

#ifndef URX_VERSION_H
#define URX_VERSION_H

#include <cstdint>
#include <iosfwd>
#include <string>

#include <urx/v0_2/object.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief The URX Version class
 */
class Version : public Object {
  URX_TYPE_MACRO(Version, Object);

 public:
  explicit Version(uint32_t major = URX_VERSION_MAJOR, uint32_t minor = URX_VERSION_MINOR,
                   uint32_t patch = URX_VERSION_PATCH) {
    m_major = major;
    m_minor = minor;
    m_patch = patch;
  }

  void printSelf(std::ostream& os, const std::string& indent) const override;

  uint32_t major() const { return m_major; }
  uint32_t minor() const { return m_minor; }
  uint32_t patch() const { return m_patch; }

  inline bool operator==(const Version& other) const {
    return ((m_major == other.m_major) && (m_minor == other.m_minor) && (m_patch == other.m_patch));
  }

  inline bool operator!=(const Version& other) const { return !(*this == other); }

 private:
  uint32_t m_major;
  uint32_t m_minor;
  uint32_t m_patch;
};

}  // namespace urx::v0_2

#endif  // URX_VERSION_H