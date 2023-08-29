#pragma once

#include <uff/trigger_in.h>
#include <memory>

struct IGroup;

namespace uff {

struct DestinationLink {
  bool operator==(const DestinationLink& other) const {
    return (_destination.expired() == other._destination.expired()) &&
           (_destination.expired() ||
            // Compare pointer instead of object to avoid recursive cycling of operator== from IGroup
            (_destination.lock().get() == other._destination.lock().get())) &&
           (_link_trigger == other._link_trigger);
  }
  inline bool operator!=(const DestinationLink& other) const { return !(*this == other); }

  std::weak_ptr<IGroup> _destination;

  TriggerIn _link_trigger;
};

}  // namespace uff
