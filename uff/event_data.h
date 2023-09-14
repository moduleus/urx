#pragma once

#include <uff/timed_event.h>

#include <memory>
#include <span>
#include <vector>

namespace uff {

template <typename T>
struct EventData {
  inline bool operator==(const EventData<T>& other) const {
    bool are_data_equaled = channel_data.size() == other.channel_data.size();
    for (size_t i = 0; i < channel_data.size() && are_data_equaled; ++i) {
      are_data_equaled &= (channel_data[i].begin() == other.channel_data[i].begin()) &&
                          (channel_data[i].end() == other.channel_data[i].end());
    }
    return are_data_equaled && (event_timestamp == other.event_timestamp);
  }
  inline bool operator!=(const EventData<T>& other) const { return !(*this == other); }

  // Data organized as _channel_data[ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  std::vector<std::span<T>> channel_data;

  double event_timestamp = std::numeric_limits<double>::max();
};

}  // namespace uff
