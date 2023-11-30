#pragma once

#include <memory>

#include <urx/group.h>

namespace urx {

struct RawData {
  std::shared_ptr<void> buffer;
  size_t size = 0;
};

}  // namespace urx
