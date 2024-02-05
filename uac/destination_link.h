#pragma once

#include <memory>
#include <optional>

#include <uac/detail/compare.h>
#include <uac/trigger.h>

#include <urx/detail/double_nan.h>

namespace uac {
struct IGroup;

struct DestinationLink {
  bool operator==(const DestinationLink& other) const {
    return trigger == other.trigger && destination == other.destination;
  }

  bool operator!=(const DestinationLink& other) const { return !operator==(other); }

  TriggerIn trigger;

  std::weak_ptr<IGroup> destination;
};

}  // namespace uac
