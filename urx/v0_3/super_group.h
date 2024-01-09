#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/group.h>
#include <urx/v0_3/igroup.h>
#include <urx/v0_3/sequence.h>

namespace urx::v0_3 {

struct SuperGroup : public IGroup {
  std::shared_ptr<IGroup> initial_group;
};

}  // namespace urx::v0_3
