#pragma once

#include <uff/destination_link.h>
#include <uff/group.h>
#include <uff/igroup.h>
#include <memory>

namespace uff {

struct SuperGroup : public IGroup {
  inline bool operator==(const SuperGroup& other) const {
    return initial_group.lock() == other.initial_group.lock();
  }
  inline bool operator!=(const SuperGroup& other) const { return !(*this == other); }

  std::weak_ptr<IGroup> initial_group;
};

}  // namespace uff
