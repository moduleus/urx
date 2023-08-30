#pragma once

#include <optional>
#include <vector>

namespace uff {

template <typename T>
struct ChannelData {
  ChannelData(std::vector<T> sample_data, std::optional<uint16_t> channel_id = std::nullopt)
      : _sample_data(std::move(sample_data)), _channel_id(channel_id) {}

  auto operator<=>(const ChannelData& other) const = default;

  // Data organized as _sample_data[ReceiveSetup::_number_samples]
  std::vector<T> _sample_data;

  std::optional<uint16_t> _channel_id;
};

}  // namespace uff
