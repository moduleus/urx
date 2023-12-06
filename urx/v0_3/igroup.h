#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/trigger_in.h>
#include <urx/v0_3/trigger_out.h>

namespace urx::v0_3 {

struct IGroup {
  std::string name;
  std::shared_ptr<TriggerIn> trigger_in;
  std::shared_ptr<TriggerOut> trigger_out;

  size_t repetition_count;
};

}  // namespace urx::v0_3
