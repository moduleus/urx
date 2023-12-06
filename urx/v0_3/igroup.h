#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/trigger_in.h>
#include <urx/v0_3/trigger_out.h>

namespace urx::v0_3 {

struct IGroup {
  double time_offset;

  std::string name;
  std::optional<TriggerIn> trigger_in;
  std::optional<TriggerOut> trigger_out;

  uint32_t repetition_count;
};

}  // namespace urx::v0_3
