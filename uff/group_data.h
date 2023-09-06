#pragma once

#include <uff/group.h>
#include <uff/sequence_data.h>

#include <complex>
#include <memory>
#include <vector>

namespace uff {

struct GroupData {
  enum class DataType { INT16 = 0, FLOAT = 1, DOUBLE = 2 };

  using VecSequenceDataType =
      std::variant<std::vector<SequenceData<int16_t>>, std::vector<SequenceData<float>>,
                   std::vector<SequenceData<double>>,
                   std::vector<SequenceData<std::complex<int16_t>>>,
                   std::vector<SequenceData<std::complex<float>>>,
                   std::vector<SequenceData<std::complex<double>>>>;
  using VecData = std::variant<std::vector<int16_t>, std::vector<float>, std::vector<double>,
                               std::vector<std::complex<int16_t>>, std::vector<std::complex<float>>,
                               std::vector<std::complex<double>>>;

  bool operator==(const GroupData& other) const {
    return (group.expired() == other.group.expired()) &&
           (group.expired() || *(group.lock()) == *(other.group.lock())) &&
           (sequence_data == other.sequence_data) && (group_timestamp == other.group_timestamp);
  }
  inline bool operator!=(const GroupData& other) const { return !(*this == other); }

  std::weak_ptr<Group> group;

  // Data type contained in the group data containers
  DataType data_type = DataType::INT16;

  VecData raw_data;

  // Data organized as _sequence_data[IGroup::_repetition_count][Sequence::_timed_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  VecSequenceDataType sequence_data;

  uint64_t group_timestamp = 0;

  // Number of bit to encode data to binary file, can be different of C++ type to optimize data space
  uint8_t size_of_data_type = 0;
};

}  // namespace uff
