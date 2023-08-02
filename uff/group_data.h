#pragma once

#include <uff/group.h>

#include <memory>
#include <vector>

namespace uff {

class GroupData {
 public:
  // CTOR & DTOR
  GroupData() = delete;
  GroupData(std::weak_ptr<Group> group, std::vector<int16_t> data, uint64_t timestamp,
            std::vector<uint64_t> sequence_timestamp,
            std::vector<std::vector<uint64_t>> event_timestamp)
      : _group(std::move(group)),
        _data(std::move(data)),
        _timestamp(timestamp),
        _sequence_timestamp(std::move(sequence_timestamp)),
        _event_timestamp(std::move(event_timestamp)) {}
  GroupData(const GroupData&) = default;
  GroupData(GroupData&&) noexcept = default;
  ~GroupData() = default;

  // Operators
  GroupData& operator=(const GroupData& other) noexcept = default;
  GroupData& operator=(GroupData&& other) noexcept = default;
  bool operator==(const GroupData& other) const {
    return (_group.expired() == other._group.expired()) &&
           (_group.expired() || *(_group.lock()) == *(other._group.lock())) &&
           (_data == other._data);
  }
  inline bool operator!=(const GroupData& other) const { return !(*this == other); }

  // Accessors
  inline std::weak_ptr<Group> group() const { return _group; }
  inline void setGroup(std::weak_ptr<Group> group) { _group = std::move(group); }

  inline std::vector<int16_t>& data() { return _data; }
  inline int16_t* pData() { return _data.data(); }
  inline void setData(std::vector<int16_t> data) { _data = std::move(data); }

  inline uint64_t timestamp() const { return _timestamp; }
  inline void setTimestamp(uint64_t timestamp) { _timestamp = timestamp; }

  inline std::vector<uint64_t>& sequenceTimestamp() { return _sequence_timestamp; }
  inline void setSequenceTimestamp(std::vector<uint64_t> sequence_timestamp) { _sequence_timestamp = std::move(sequence_timestamp); }

  inline std::vector<std::vector<uint64_t>>& eventTimestamp() { return _event_timestamp; }
  inline void setEventTimestamp(std::vector<std::vector<uint64_t>> event_timestamp) { _event_timestamp = std::move(event_timestamp); }

  // Members
 private:
  std::weak_ptr<Group> _group;

  // Data organized as data[iFrame][iEvent][iChannel][iSample].
  std::vector<int16_t> _data;

  uint64_t _timestamp = 0;

  std::vector<uint64_t> _sequence_timestamp;

  std::vector<std::vector<uint64_t>> _event_timestamp;
};

}  // namespace uff
