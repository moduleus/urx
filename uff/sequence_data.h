#pragma once

#include <uff/event_data.h>
#include <uff/group.h>

#include <memory>
#include <optional>
#include <vector>

namespace uff {

template <typename T>
struct SequenceData {
  auto operator<=>(const SequenceData& other) const = default;

  // Data organized as _event_data[Sequence::_timed_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  std::vector<EventData<T>> event_data;

  uint64_t sequence_timestamp = 0;

  std::optional<uint16_t> sequence_id;
};

}  // namespace uff
