#pragma once

#include <uff/group.h>
#include <uff/sequence_data.h>

#include <memory>
#include <vector>

namespace uff {

struct GroupData {
  GroupData(std::weak_ptr<Group> group, std::vector<SequenceData> sequence_data,
            uint64_t group_timestamp)
      : _group(std::move(group)),
        _sequence_data(std::move(sequence_data)),
        _group_timestamp(group_timestamp) {}

  bool operator==(const GroupData& other) const {
    return (_group.expired() == other._group.expired()) &&
           (_group.expired() || *(_group.lock()) == *(other._group.lock())) &&
           (_sequence_data == other._sequence_data) && (_group_timestamp == other._group_timestamp);
  }
  inline bool operator!=(const GroupData& other) const { return !(*this == other); }

  std::weak_ptr<Group> _group;

  // Data organized as _sequence_data[IGroup::_repetition_count][Sequence::_timed_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  std::vector<SequenceData> _sequence_data;

  uint64_t _group_timestamp = 0;
};

}  // namespace uff
