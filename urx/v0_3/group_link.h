#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/igroup.h>
#include <urx/v0_3/trigger_in.h>

namespace urx::v0_3 {

struct GroupLink {
  std::shared_ptr<IGroup> source;
  std::shared_ptr<IGroup> destination;
  std::shared_ptr<TriggerIn> trigger_in;
};

}  // namespace urx::v0_3
