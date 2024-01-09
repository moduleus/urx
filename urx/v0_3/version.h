#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <urx/v0_3/urx.h>

namespace urx::v0_3 {

struct Version {
  uint16_t major = URX_VERSION_MAJOR;
  uint16_t minor = URX_VERSION_MINOR;
  uint16_t patch = URX_VERSION_PATCH;
  std::string specialization;
};

}  // namespace urx::v0_3
