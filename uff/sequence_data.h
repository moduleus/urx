#pragma once

#include <uff/event_data.h>
#include <uff/group.h>

#include <memory>
#include <optional>
#include <vector>

namespace uff {

template <typename T>
struct SequenceData {
  SequenceData(std::vector<EventData<T>> event_data, uint64_t sequence_timestamp,
               std::optional<uint16_t> sequence_id = std::nullopt)
      : _event_data(std::move(event_data)),
        _sequence_timestamp(sequence_timestamp),
        _sequence_id(sequence_id) {}

  inline bool operator==(const SequenceData<T>& other) const {
    return (_event_data == other._event_data) &&
           (_sequence_timestamp == other._sequence_timestamp) &&
           (_sequence_id == other._sequence_id);
  }

  inline bool operator!=(const SequenceData& other) const { return !(*this == other); }

  // Data organized as _event_data[Sequence::_timed_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  std::vector<EventData<T>> _event_data;

  uint64_t _sequence_timestamp = 0;

  std::optional<uint16_t> _sequence_id;
};

}  // namespace uff
