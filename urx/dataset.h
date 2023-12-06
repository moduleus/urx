#pragma once

#include <memory>

#include <urx/acquisition.h>
#include <urx/version.h>

namespace urx {

// Root class. See wiki for documentation.

struct Dataset {
  bool operator==(const Dataset& other) const = default;

  Version version;

  Acquisition acquisition;
};

}  // namespace urx
