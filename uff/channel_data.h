#pragma once

#include <optional>
#include <span>

namespace uff {

template <typename T>
struct ChannelData {
  auto operator<=>(const ChannelData& other) const = default;

  // Data organized as _sample_data[ReceiveSetup::_number_samples]
  std::span<T> sample_data;

  std::optional<uint16_t> channel_id;
};

}  // namespace uff
