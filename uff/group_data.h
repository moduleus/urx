#pragma once

#include <uff/group.h>
#include <uff/sequence_data.h>

#include <complex>
#include <memory>
#include <variant>
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
    size_t group_data_size = 0u;
    size_t other_group_data_size = 0u;

    bool are_sequence_data_equaled =
        data_type == other.data_type &&
        std::is_same_v<decltype(sequence_data), decltype(other.sequence_data)>;
    if (are_sequence_data_equaled) {
      std::visit(
          [&other_sequence_data_variant = other.sequence_data, &group_data_size,
           &other_group_data_size, &are_sequence_data_equaled](auto&& this_sequence_data) {
            using sequence_data_type =
                std::remove_const_t<std::remove_reference_t<decltype(this_sequence_data)>>;
            auto& other_sequence_data = std::get<sequence_data_type>(other_sequence_data_variant);

            group_data_size = this_sequence_data.size();
            other_group_data_size = other_sequence_data.size();

            are_sequence_data_equaled &= group_data_size == other_group_data_size;
            for (uint32_t i = 0; i < group_data_size && are_sequence_data_equaled; ++i) {
              are_sequence_data_equaled =
                  are_sequence_data_equaled && (this_sequence_data[i] == other_sequence_data[i]);
            }
          },
          sequence_data);
    }

    return (group.expired() == other.group.expired()) &&
           (group.expired() || *(group.lock()) == *(other.group.lock())) &&
           (raw_data == other.raw_data) && (size_of_data_type == other.size_of_data_type) &&
           (group_timestamp == other.group_timestamp) && are_sequence_data_equaled;
  }
  inline bool operator!=(const GroupData& other) const { return !(*this == other); }

  std::weak_ptr<Group> group;

  // Data type contained in the group data containers
  DataType data_type = DataType::INT16;

  VecData raw_data;

  // Data organized as _sequence_data[IGroup::_repetition_count][Sequence::_timed_events.size()][ReceiveSetup::_channel_mapping.size()][ReceiveSetup::_number_samples]
  VecSequenceDataType sequence_data;

  double group_timestamp = std::numeric_limits<double>::max();

  // Number of bit to encode data to binary file, can be different of C++ type to optimize data space
  uint8_t size_of_data_type = 0;
};

}  // namespace uff
