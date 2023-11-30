#pragma once

#include <memory>

#include <uff/group.h>

namespace uff {

struct RawData {
  std::shared_ptr<void> buffer;
  size_t size = 0;
};

}  // namespace uff
