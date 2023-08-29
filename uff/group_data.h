#pragma once

#include <uff/group.h>

#include <memory>
#include <vector>

namespace uff {

struct GroupData {
  GroupData(std::weak_ptr<Group> group, std::vector<int16_t> data, uint64_t timestamp,
            std::vector<uint64_t> sequence_timestamp,
            std::vector<std::vector<uint64_t>> event_timestamp)
      : _group(std::move(group)),
        _data(std::move(data)),
        _timestamp(timestamp),
        _sequence_timestamp(std::move(sequence_timestamp)),
        _event_timestamp(std::move(event_timestamp)) {}

  bool operator==(const GroupData& other) const {
    return (_group.expired() == other._group.expired()) &&
           (_group.expired() || *(_group.lock()) == *(other._group.lock())) &&
           (_data == other._data);
  }
  inline bool operator!=(const GroupData& other) const { return !(*this == other); }

  std::weak_ptr<Group> _group;

  // Data organized as data[iFrame][iEvent][iChannel][iSample].
  std::vector<int16_t> _data;

  uint64_t _timestamp = 0;

  std::vector<uint64_t> _sequence_timestamp;

  std::vector<std::vector<uint64_t>> _event_timestamp;
};

}  // namespace uff
