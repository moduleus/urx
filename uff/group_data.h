#pragma once

#include <uff/group.h>

#include <complex>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

namespace uff {

struct GroupData {
  enum class DataType { INT16 = 0, INT32 = 1, FLOAT = 2, DOUBLE = 3 };

  template <class... Args>
  struct VecDataType {
    using real_and_complex = std::variant<std::vector<Args>..., std::vector<std::complex<Args>>...>;
  };

  using VecDataTypeVariant = VecDataType<int16_t, int32_t, float, double>::real_and_complex;

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

  // Data organized as _sequence_data[IGroup::_repetition_count][Sequence::_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples] but in 1D array
  // Create your helper or use Uff_Utils to access correctly to the data
  VecDataTypeVariant raw_data;

  double group_timestamp = UFF_NAN;

  std::vector<double> sequence_timestamps;

  std::vector<std::vector<double>> event_timestamps;

  // Number of bit to encode data to binary file, can be different of C++ type to optimize data space
  uint8_t size_of_data_type = 0;

  void printData() {
    auto printVec = []<typename T>(std::vector<T> vec) -> void {
      for (auto v : vec) std::cout << v << "; ";
      std::cout << std::endl;
    };
    std::visit([&printVec](auto&& vec) { printVec(vec); }, raw_data);
  }

  template <typename T>
  bool checkType() {
    if (auto d = std::get_if<T>(&raw_data)) {
      return true;
    }
    return false;
  }
};

}  // namespace uff
