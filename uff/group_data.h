#pragma once

#include <uff/group.h>
#include <uff/sequence_data.h>

#include <memory>
#include <vector>

namespace uff {

template <typename T>
struct GroupData {
  bool operator==(const GroupData<T>& other) const {
    return (group.expired() == other.group.expired()) &&
           (group.expired() || *(group.lock()) == *(other.group.lock())) &&
           (sequence_data == other.sequence_data) && (group_timestamp == other.group_timestamp);
  }
  inline bool operator!=(const GroupData<T>& other) const { return !(*this == other); }

  std::weak_ptr<Group> group;

  // Data organized as _sequence_data[IGroup::_repetition_count][Sequence::_timed_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  std::vector<SequenceData<T>> sequence_data;

  uint64_t group_timestamp = 0;
};

}  // namespace uff
