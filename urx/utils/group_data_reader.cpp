#include <algorithm>

#include <urx/detail/raw_data.h>
#include <urx/event.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/receive_setup.h>
#include <urx/utils/common.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/group_helper.h>

namespace urx::utils {

GroupDataReader::GroupDataReader(GroupData& group_data, const size_t custom_sample_byte_size)
    : group_data_{group_data} {
  try {
    auto group = group_data_.group.lock();
    sample_byte_size_ = GroupHelper::sizeof_sample(group->sampling_type, group->data_type);
  } catch (std::out_of_range&) {
    sample_byte_size_ = custom_sample_byte_size;
  }

  samples_offset_.emplace_back(0);

  for (const auto& event : group_data_.group.lock()->sequence) {
    const auto& receive_setup = event.receive_setup;
    const size_t channels_count = map_max(receive_setup.channel_mapping);
    const size_t samples_count = receive_setup.number_samples;
    const size_t samples_offset = channels_count * samples_count;

    samples_offset_.emplace_back(samples_offset + samples_offset_.back());
    samples_count_.emplace_back(samples_count);
  }
}

size_t GroupDataReader::sequencesCount() const {
  return group_data_.raw_data->getSize() / samples_offset_.back();
}

size_t GroupDataReader::eventsCount() const { return group_data_.group.lock()->sequence.size(); }

size_t GroupDataReader::channelsCount(const size_t event_idx) const {
  return (samples_offset_[event_idx + 1] - samples_offset_[event_idx]) / samplesCount(event_idx);
}

size_t GroupDataReader::samplesCount(const size_t event_idx) const {
  return samples_count_[event_idx];
}

size_t GroupDataReader::offset(const size_t sequence_idx, const size_t event_idx,
                               const size_t channel_idx, const size_t sample_idx) const {
  return sequence_idx * samples_offset_.back() + samples_offset_[event_idx] +
         channel_idx * samples_count_[event_idx] + sample_idx;
}

size_t GroupDataReader::sampleByteSize() const { return sample_byte_size_; }

}  // namespace urx::utils