#pragma once

#include <uff/group_data.h>
#include <variant>
#include <vector>

namespace uff {

size_t elementByteSize(GroupData& group_data) {
  size_t byte_size = 0;

  switch (group_data.data_type) {
    case GroupData::DataType::INT16:
      byte_size = sizeof(int16_t);
      break;
    case GroupData::DataType::INT32:
      byte_size = sizeof(int32_t);
      break;
    case GroupData::DataType::FLOAT:
      byte_size = sizeof(float);
      break;
    case GroupData::DataType::DOUBLE:
      byte_size = sizeof(double);
      break;
    default:
      throw std::runtime_error("Not handled data type enum value");
  }

  if (group_data.group->sampling_type == Group::SamplingType::IQ) byte_size *= 2;

  return byte_size;
}

class GroupDataReader {
 public:
  GroupDataReader(GroupData& group_data) : _group_data{group_data} {
    _samples_offset.emplace_back(0);

    for (auto event : _group_data.group->sequence) {
      auto& receive_setup = *event.receive_setup;
      const size_t samples_count = receive_setup.number_samples;
      const size_t samples_offset = receive_setup.channel_mapping.size() * samples_count;

      _samples_offset.emplace_back(samples_offset + _samples_offset.back());
      _samples_count.emplace_back(samples_count);
    }

    std::cout << "samples offset [ ";
    for (auto o : _samples_offset) std::cout << o << " ";
    std::cout << "]" << std::endl;
  }

  template <typename T>
  T& at(const size_t frame_idx, const size_t event_idx, const size_t channel_idx,
        const size_t sample_idx) {
    auto& data = std::get<std::vector<T>>(_group_data.raw_data);
    return reinterpret_cast<T*>(data.data())[offset(frame_idx, event_idx, channel_idx, sample_idx)];
  }

  size_t size() {
    return std::visit([](auto&& vec) { return vec.size(); }, _group_data.raw_data);
  }

  size_t framesCount() { return size() / _samples_offset.back(); }

  size_t eventsCount() { return _group_data.group->sequence.size(); }

  size_t channelsCount(const size_t event_idx) {
    return (_samples_offset[event_idx + 1] - _samples_offset[event_idx]) / samplesCount(event_idx);
  }

  size_t samplesCount(const size_t event_idx) { return _samples_count[event_idx]; }

  size_t offset(const size_t frame_idx, const size_t event_idx, const size_t channel_idx,
                const size_t sample_idx) const {
    return frame_idx * _samples_offset.back() + _samples_offset[event_idx] +
           channel_idx * _samples_count[event_idx] + sample_idx;
  }

 private:
  GroupData& _group_data;

  std::vector<size_t> _samples_offset;  // Samples offset relative to an event
  std::vector<size_t> _samples_count;   // Number of samples in channels relative to an event
};

}  // namespace uff