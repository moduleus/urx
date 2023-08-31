#pragma once

#include <uff/trigger_in.h>
#include <memory>

struct IGroup;

namespace uff {

struct DestinationLink {
  bool operator==(const DestinationLink& other) const {
    return (destination.expired() == other.destination.expired()) &&
           (destination.expired() ||
            // Compare pointer instead of object to avoid recursive cycling of operator== from IGroup
            (destination.lock().get() == other.destination.lock().get())) &&
           (link_trigger == other.link_trigger);
  }
  inline bool operator!=(const DestinationLink& other) const { return !(*this == other); }

  std::weak_ptr<IGroup> destination;

  TriggerIn link_trigger;
};

}  // namespace uff
