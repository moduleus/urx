#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <urx/urx.h>

namespace urx {

struct Version {
  bool operator==(const Version& other) const = default;

  uint16_t major = URX_VERSION_MAJOR;
  uint16_t minor = URX_VERSION_MINOR;
  uint16_t patch = URX_VERSION_PATCH;
};

}  // namespace urx
