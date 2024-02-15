#include <algorithm>

#include <urx/event.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/receive_setup.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/group_helper.h>

namespace urx::utils {

GroupDataReader::GroupDataReader(const GroupData& group_data, const size_t custom_sample_byte_size)
    : _group_data{group_data} {
  try {
    auto group = _group_data.group.lock();
    _sample_byte_size = group_helper::sizeofSample(group->sampling_type, group->data_type);
  } catch (std::out_of_range&) {
    _sample_byte_size = custom_sample_byte_size;
  }

  _samples_offset.emplace_back(0);

  for (const auto& event : _group_data.group.lock()->sequence) {
    const auto& receive_setup = event.receive_setup;
    const size_t channels_count = receive_setup.active_elements.size();
    const size_t samples_count = receive_setup.number_samples;
    const size_t samples_offset = channels_count * samples_count;

    _samples_offset.emplace_back(samples_offset + _samples_offset.back());
    _samples_count.emplace_back(samples_count);
  }
}

size_t GroupDataReader::sequencesCount() const {
  return _group_data.raw_data->getSize() / _samples_offset.back();
}

size_t GroupDataReader::eventsCount() const { return _group_data.group.lock()->sequence.size(); }

size_t GroupDataReader::channelsCount(const size_t event_idx) const {
  return (_samples_offset[event_idx + 1] - _samples_offset[event_idx]) / samplesCount(event_idx);
}

size_t GroupDataReader::samplesCount(const size_t event_idx) const {
  return _samples_count[event_idx];
}

size_t GroupDataReader::offset(const size_t sequence_idx, const size_t event_idx,
                               const size_t channel_idx, const size_t sample_idx) const {
  return sequence_idx * _samples_offset.back() + _samples_offset[event_idx] +
         channel_idx * _samples_count[event_idx] + sample_idx;
}

size_t GroupDataReader::sampleByteSize() const { return _sample_byte_size; }

}  // namespace urx::utils