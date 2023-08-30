#pragma once

#include <uff/channel_data.h>
#include <uff/timed_event.h>

#include <memory>
#include <vector>

namespace uff {

template <typename T>
struct EventData {
  EventData(std::vector<ChannelData<T>> channel_data, uint64_t event_timestamp,
            const std::weak_ptr<TimedEvent>& event = std::weak_ptr<TimedEvent>())
      : _channel_data(std::move(channel_data)), _event_timestamp(event_timestamp), _event(event) {}

  inline bool operator==(const EventData<T>& other) const {
    return (_event.expired() == other._event.expired()) &&
           (_event.expired() || *(_event.lock()) == *(other._event.lock())) &&
           (_channel_data == other._channel_data) && (_event_timestamp == other._event_timestamp);
  }
  inline bool operator!=(const EventData<T>& other) const { return !(*this == other); }

  // Data organized as _channel_data[ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  std::vector<ChannelData<T>> _channel_data;

  uint64_t _event_timestamp = 0;

  std::weak_ptr<TimedEvent> _event;
};

}  // namespace uff
