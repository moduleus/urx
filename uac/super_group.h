#pragma once

#include <memory>

#include <uac/detail/compare.h>
#include <uac/igroup.h>

namespace uac {

struct SuperGroup : IGroup {
  bool operator==(const SuperGroup& other) const {
    return IGroup::operator==(other) && initial_group == other.initial_group;
  }

  bool operator!=(const SuperGroup& other) const { return !operator==(other); }

  ~SuperGroup() override = default;

  std::weak_ptr<IGroup> initial_group;
};

}  // namespace uac
