#pragma once

#include <uff/channel_data.h>
#include <uff/timed_event.h>

#include <memory>
#include <vector>

namespace uff {

template <typename T>
struct EventData {
  inline bool operator==(const EventData<T>& other) const {
    return (event.expired() == other.event.expired()) &&
           (event.expired() || *(event.lock()) == *(other.event.lock())) &&
           (channel_data == other.channel_data) && (event_timestamp == other.event_timestamp);
  }
  inline bool operator!=(const EventData<T>& other) const { return !(*this == other); }

  // Data organized as _channel_data[ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  std::vector<ChannelData<T>> channel_data;

  double event_timestamp = std::numeric_limits<double>::max();

  std::weak_ptr<TimedEvent> event;
};

}  // namespace uff
