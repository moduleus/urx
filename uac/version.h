#pragma once

#include <uac/uac.h>

#include <urx/version.h>

namespace uac {

struct Version : urx::Version {
  Version() : urx::Version{UAC_VERSION_MAJOR, UAC_VERSION_MINOR, UAC_VERSION_PATCH} {}
};

}  // namespace uac
