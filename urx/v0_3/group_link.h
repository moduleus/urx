#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/igroup.h>
#include <urx/v0_3/trigger_in.h>

namespace urx::v0_3 {

struct GroupLink {
  std::weak_ptr<IGroup> source;
  std::weak_ptr<IGroup> destination;
  std::optional<TriggerIn> trigger_in;
};

}  // namespace urx::v0_3
