#pragma once

#include <optional>
#include <span>

namespace uff {

template <typename T>
struct ChannelData {
  inline bool operator==(const ChannelData<T>& other) const {
    return (sample_data.begin() == other.sample_data.begin()) &&
           (sample_data.end() == other.sample_data.end()) && (channel_id == other.channel_id);
  }
  inline bool operator!=(const ChannelData<T>& other) const { return !(*this == other); }

  // Data organized as _sample_data[ReceiveSetup::_number_samples]
  std::span<T> sample_data;

  std::optional<uint16_t> channel_id;
};

}  // namespace uff
