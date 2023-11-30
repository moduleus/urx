#pragma once

#include <complex>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

#include <uff/detail/compare.h>
#include <uff/detail/double_nan.h>
#include <uff/detail/raw_data.h>
#include <uff/group.h>

namespace uff {

/**
 * @brief The UFF class containing all the retrieved data from the acquisition
 */
struct GroupData {
  bool operator==(const GroupData& other) const {
    static std::unordered_map<Group::DataType, size_t> group_dt_to_sizeof{
        {Group::DataType::INT16, sizeof(int16_t)},
        {Group::DataType::INT32, sizeof(int32_t)},
        {Group::DataType::FLOAT, sizeof(float)},
        {Group::DataType::DOUBLE, sizeof(double)}};

    const std::shared_ptr<Group> ptr_locked = group.lock();

    return group == other.group && raw_data.size == other.raw_data.size &&
           group_timestamp == other.group_timestamp &&
           sequence_timestamps == other.sequence_timestamps &&
           event_timestamps == other.event_timestamps &&
           (ptr_locked &&
            std::memcmp(raw_data.buffer.get(), other.raw_data.buffer.get(),
                        raw_data.size * group_dt_to_sizeof.at(ptr_locked->data_type) *
                            (ptr_locked->sampling_type == Group::SamplingType::RF ? 1 : 2)) == 0);
  }

  /// Reference of the group whose data have been retrieved
  std::weak_ptr<Group> group = std::weak_ptr<Group>();

  /// Data are organized as raw_data[Group repetition count][Number of event][Number of channel activated during the event][Number of samples]
  /// Data are in 1D array since the dimensions of the data array is dynamic for the number of activated channels and for the number of samples
  /// Create your helper to deduce the dimensions or use UffUtils to access correctly to the data
  RawData raw_data;

  /// Timestamp of the group launch [s]
  DoubleNan group_timestamp;

  /// Timestamp list for each sequence launch [s]
  std::vector<double> sequence_timestamps;

  /// Timestamp 2D list for each event of each sequence launch [s]
  std::vector<std::vector<double>> event_timestamps;
};

}  // namespace uff
