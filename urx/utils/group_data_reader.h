#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

#include <urx/group_data.h>

namespace urx::utils {

class GroupDataReader {
 public:
  explicit GroupDataReader(const GroupData& group_data, const size_t custom_sample_byte_size = 1);

  GroupDataReader(GroupDataReader&& other) noexcept = delete;
  GroupDataReader(GroupDataReader const& other) = delete;
  GroupDataReader& operator=(GroupDataReader&& other) noexcept = delete;
  GroupDataReader& operator=(GroupDataReader const& other) = delete;

  template <class T = void>
  T* operator()(const size_t sequence_idx = 0, const size_t event_idx = 0,
                const size_t channel_idx = 0, const size_t sample_idx = 0) {
    if constexpr (!std::is_same_v<T, void>) {
      if (sizeof(T) != _sample_byte_size) throw std::runtime_error("Invalid cast with raw_data");
    }

    char* byte_data = reinterpret_cast<char*>(_group_data.raw_data->getBuffer());
    return reinterpret_cast<T*>(
        byte_data + offset(sequence_idx, event_idx, channel_idx, sample_idx) * _sample_byte_size);
  }

  template <typename T>
  T& at(const size_t sequence_idx, const size_t event_idx, const size_t channel_idx,
        const size_t sample_idx) {
    // NOLINTNEXTLINE(readability-suspicious-call-argument)
    return *operator()<T>(sequence_idx, event_idx, channel_idx, sample_idx);
  }

  size_t sequencesCount() const;

  size_t eventsCount() const;

  size_t channelsCount(const size_t event_idx) const;

  size_t samplesCount(const size_t event_idx) const;

  size_t offset(const size_t sequence_idx, const size_t event_idx, const size_t channel_idx,
                const size_t sample_idx) const;

  size_t sampleByteSize() const;

 private:
  const GroupData& _group_data;

  size_t _sample_byte_size;             // Number of bytes per sample
  std::vector<size_t> _samples_offset;  // Samples offset relative to an event
  std::vector<size_t> _samples_count;   // Number of samples in channels relative to an event
};

}  // namespace urx::utils