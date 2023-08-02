#pragma once

#include <uff/uff.h>

#include <cstdint>
#include <string>

namespace uff {

class Version {
 public:
  // CTOR & DTOR
  Version() = default;
  explicit Version(uint16_t major, uint16_t minor, uint16_t patch)
      : _major(major), _minor(minor), _patch(patch) {}
  Version(const Version&) = default;
  Version(Version&&) = default;
  ~Version() = default;

  // Operators
  Version& operator=(const Version&) noexcept = default;
  Version& operator=(Version&& other) noexcept = default;
  inline bool operator==(const Version& other) const {
    return _major == other.major() && _minor == other.minor() && _patch == other.patch();
  };
  inline bool operator!=(const Version& other) const { return !(*this == other); };

  // Accessors
  inline uint16_t major() const { return _major; }
  inline uint16_t minor() const { return _minor; }
  inline uint16_t patch() const { return _patch; }

  // Members
 private:
  uint16_t _major = UFF_VERSION_MAJOR;
  uint16_t _minor = UFF_VERSION_MINOR;
  uint16_t _patch = UFF_VERSION_PATCH;
};

}  // namespace uff
