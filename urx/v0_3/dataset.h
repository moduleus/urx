#pragma once

#include <memory>

#include <urx/v0_3/acquisition.h>
#include <urx/v0_3/version.h>

namespace urx::v0_3 {

struct Dataset {
  Version version;

  Acquisition acquisition;
};

}  // namespace urx::v0_3
