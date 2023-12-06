#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/group.h>

namespace urx::v0_3 {

struct GroupData {
  Group group;
  std::shared_ptr<void> data;
  uint64_t timestamp;
  std::vector<uint64_t> sequence_timestamps;
  std::vector<std::vector<uint64_t>> event_timestamps;
};

}  // namespace urx::v0_3
