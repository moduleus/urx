#pragma once

#include <uff/group.h>

#include <complex>
#include <memory>
#include <variant>
#include <vector>

namespace uff {

struct GroupData {
  enum class DataType { INT16 = 0, INT32 = 1, FLOAT = 2, DOUBLE = 3 };
  using VecData = std::variant<std::vector<int16_t>, std::vector<int32_t>, std::vector<float>,
                               std::vector<double>, std::vector<std::complex<int16_t>>,
                               std::vector<std::complex<int32_t>>, std::vector<std::complex<float>>,
                               std::vector<std::complex<double>>>;

  bool operator==(const GroupData& other) const {
    return (group.expired() == other.group.expired()) &&
           (group.expired() || *(group.lock()) == *(other.group.lock())) &&
           (raw_data == other.raw_data) && (size_of_data_type == other.size_of_data_type) &&
           (group_timestamp == other.group_timestamp) &&
           (sequence_timestamps == other.sequence_timestamps) &&
           (event_timestamps == other.event_timestamps);
  }
  inline bool operator!=(const GroupData& other) const { return !(*this == other); }

  std::weak_ptr<Group> group;

  // Data type contained in the group data containers
  DataType data_type = DataType::INT16;

  // Data organized as _sequence_data[IGroup::_repetition_count][Sequence::_timed_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples] but in 1D array
  // Create your helper or use Uff_Utils to access correctly to the data
  VecData raw_data;

  double group_timestamp = UFF_NAN;

  std::vector<double> sequence_timestamps;

  std::vector<std::vector<double>> event_timestamps;

  // Number of bit to encode data to binary file, can be different of C++ type to optimize data space
  uint8_t size_of_data_type = 0;
};

}  // namespace uff
