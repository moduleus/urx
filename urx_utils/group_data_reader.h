#pragma once

#include <variant>
#include <vector>

#include <urx/group_data.h>
#include <urx_utils/group_helper.h>

namespace urx {

inline size_t elementByteSize(const Group& group) {
  size_t byte_size = 0;

  switch (group.data_type) {
    case Group::DataType::INT16:
      byte_size = sizeof(int16_t);
      break;
    case Group::DataType::INT32:
      byte_size = sizeof(int32_t);
      break;
    case Group::DataType::FLOAT:
      byte_size = sizeof(float);
      break;
    case Group::DataType::DOUBLE:
      byte_size = sizeof(double);
      break;
    default:
      throw std::runtime_error("Not handled data type enum value");
  }

  if (group.sampling_type == Group::SamplingType::IQ) byte_size *= 2;

  return byte_size;
}

class GroupDataReader {
 public:
  explicit GroupDataReader(GroupData& group_data) : _group_data{group_data} {
    _samples_offset.emplace_back(0);

    for (const auto& event : _group_data.group.lock()->sequence) {
      const auto& receive_setup = event.receive_setup;
      const size_t samples_count = receive_setup.number_samples;
      const size_t samples_offset = receive_setup.channel_mapping.size() * samples_count;

      _samples_offset.emplace_back(samples_offset + _samples_offset.back());
      _samples_count.emplace_back(samples_count);
    }

    std::cout << "samples offset [ ";
    for (auto o : _samples_offset) std::cout << o << " ";
    std::cout << "]\n";
  }

  GroupDataReader(GroupDataReader&& other) noexcept = delete;
  GroupDataReader(GroupDataReader const& other) = delete;
  GroupDataReader& operator=(GroupDataReader&& other) noexcept = delete;
  GroupDataReader& operator=(GroupDataReader const& other) = delete;

  template <typename T>
  T& at(const size_t sequence_idx, const size_t event_idx, const size_t channel_idx,
        const size_t sample_idx) {
    void* data = _group_data.raw_data->getBuffer();
    const std::shared_ptr<Group> group = _group_data.group.lock();

    if constexpr (std::is_same_v<T, int16_t>) {
      if (group->data_type != Group::DataType::INT16 ||
          group->sampling_type != Group::SamplingType::RF) {
        throw std::runtime_error("Invalid cast with raw_data");
      }
    }
    if constexpr (std::is_same_v<T, int32_t>) {
      if (group->data_type != Group::DataType::INT32 ||
          group->sampling_type != Group::SamplingType::RF) {
        throw std::runtime_error("Invalid cast with raw_data");
      }
    }
    if constexpr (std::is_same_v<T, float>) {
      if (group->data_type != Group::DataType::FLOAT ||
          group->sampling_type != Group::SamplingType::RF) {
        throw std::runtime_error("Invalid cast with raw_data");
      }
    }
    if constexpr (std::is_same_v<T, double>) {
      if (group->data_type != Group::DataType::DOUBLE ||
          group->sampling_type != Group::SamplingType::RF) {
        throw std::runtime_error("Invalid cast with raw_data");
      }
    }
    if constexpr (std::is_same_v<T, std::complex<int16_t>>) {
      if (group->data_type != Group::DataType::INT16 ||
          group->sampling_type != Group::SamplingType::IQ) {
        throw std::runtime_error("Invalid cast with raw_data");
      }
    }
    if constexpr (std::is_same_v<T, std::complex<int32_t>>) {
      if (group->data_type != Group::DataType::INT32 ||
          group->sampling_type != Group::SamplingType::IQ) {
        throw std::runtime_error("Invalid cast with raw_data");
      }
    }
    if constexpr (std::is_same_v<T, std::complex<float>>) {
      if (group->data_type != Group::DataType::FLOAT ||
          group->sampling_type != Group::SamplingType::IQ) {
        throw std::runtime_error("Invalid cast with raw_data");
      }
    }
    if constexpr (std::is_same_v<T, std::complex<double>>) {
      if (group->data_type != Group::DataType::DOUBLE ||
          group->sampling_type != Group::SamplingType::IQ) {
        throw std::runtime_error("Invalid cast with raw_data");
      }
    }

    return static_cast<T*>(data)[offset(sequence_idx, event_idx, channel_idx, sample_idx)];
  }

  size_t sequencesCount() const { return _group_data.raw_data->getSize() / _samples_offset.back(); }

  size_t eventsCount() const { return _group_data.group.lock()->sequence.size(); }

  size_t channelsCount(const size_t event_idx) const {
    return (_samples_offset[event_idx + 1] - _samples_offset[event_idx]) / samplesCount(event_idx);
  }

  size_t samplesCount(const size_t event_idx) const { return _samples_count[event_idx]; }

  size_t offset(const size_t sequence_idx, const size_t event_idx, const size_t channel_idx,
                const size_t sample_idx) const {
    return sequence_idx * _samples_offset.back() + _samples_offset[event_idx] +
           channel_idx * _samples_count[event_idx] + sample_idx;
  }

 private:
  GroupData& _group_data;

  std::vector<size_t> _samples_offset;  // Samples offset relative to an event
  std::vector<size_t> _samples_count;   // Number of samples in channels relative to an event
};

}  // namespace urx