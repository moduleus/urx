#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/rotation.h>
#include <urx/v0_3/translation.h>

namespace urx::v0_3 {

struct Transform {
  Translation translation;
  Rotation rotation;
};

}  // namespace urx::v0_3
