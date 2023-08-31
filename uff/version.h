#pragma once

#include <uff/uff.h>

#include <cstdint>
#include <string>

namespace uff {

struct Version {
  auto operator<=>(const Version& other) const = default;

  uint16_t major = UFF_VERSION_MAJOR;
  uint16_t minor = UFF_VERSION_MINOR;
  uint16_t patch = UFF_VERSION_PATCH;
};

}  // namespace uff
