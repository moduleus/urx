#pragma once

#include <uac/event.h>
#include <uac/igroup.h>

#include <urx/group.h>

namespace uac {

struct Group : urx::detail::Group<Event>, IGroup {
  ~Group() override = default;

  bool operator==(const Group& other) const {
    return urx::detail::Group<Event>::operator==(other) && IGroup::operator==(other);
  }

  bool operator!=(const Group& other) const { return !operator==(other); }
};

}  // namespace uac
