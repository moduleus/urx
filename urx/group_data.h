#pragma once

#include <complex>
#include <cstring>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/group.h>

namespace urx {

struct GroupData {
  bool operator==(const GroupData& other) const {
    static std::unordered_map<Group::DataType, size_t> group_dt_to_sizeof{
        {Group::DataType::INT16, sizeof(int16_t)},
        {Group::DataType::INT32, sizeof(int32_t)},
        {Group::DataType::FLOAT, sizeof(float)},
        {Group::DataType::DOUBLE, sizeof(double)}};

    const std::shared_ptr<Group> ptr_locked = group.lock();

    return group == other.group && raw_data->getSize() == other.raw_data->getSize() &&
           group_timestamp == other.group_timestamp &&
           sequence_timestamps == other.sequence_timestamps &&
           event_timestamps == other.event_timestamps &&
           (ptr_locked &&
            std::memcmp(raw_data->getBuffer(), other.raw_data->getBuffer(),
                        raw_data->getSize() * group_dt_to_sizeof.at(ptr_locked->data_type) *
                            (ptr_locked->sampling_type == Group::SamplingType::RF ? 1 : 2)) == 0);
  }

  std::weak_ptr<Group> group = std::weak_ptr<Group>();
  std::shared_ptr<RawData> raw_data;

  DoubleNan group_timestamp;

  std::vector<double> sequence_timestamps;
  std::vector<std::vector<double>> event_timestamps;
};

}  // namespace urx
