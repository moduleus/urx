#pragma once

#include <uff/group.h>

#include <memory>
#include <optional>
#include <span>
#include <vector>

namespace uff {

template <typename T>
struct SequenceData {
  inline bool operator==(const SequenceData<T>& other) const {
    bool are_data_equaled = event_data.size() == other.event_data.size();
    for (size_t i = 0; i < event_data.size() && are_data_equaled; ++i) {
      const auto& channel_data = event_data[i];
      const auto& other_channel_data = other.event_data[i];
      are_data_equaled &= channel_data.size() == other_channel_data.size();

      for (size_t j = 0; j < channel_data.size() && are_data_equaled; ++j) {
        are_data_equaled &= (channel_data[j].begin() == other_channel_data[j].begin()) &&
                            (channel_data[j].end() == other_channel_data[j].end());
      }
    }
    return are_data_equaled && (event_timestamps == other.event_timestamps) &&
           (sequence_timestamp == other.sequence_timestamp);
  }
  inline bool operator!=(const SequenceData<T>& other) const { return !(*this == other); }

  // Data organized as _event_data[Sequence::_timed_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  std::vector<std::vector<std::span<T>>> event_data;

  std::vector<double> event_timestamps;

  double sequence_timestamp = UFF_NAN;
};

}  // namespace uff
