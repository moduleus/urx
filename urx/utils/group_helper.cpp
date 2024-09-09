#include <cstdint>
#include <memory>
#include <unordered_map>

#include <urx/enums.h>
#include <urx/utils/group_helper.h>

namespace urx::utils::group_helper {

size_t sizeofDataType(const DataType& data_type) {
  static std::unordered_map<DataType, size_t> group_dt_to_sizeof{
      {DataType::INT16, sizeof(int16_t)},
      {DataType::INT32, sizeof(int32_t)},
      {DataType::FLOAT, sizeof(float)},
      {DataType::DOUBLE, sizeof(double)}};

  return group_dt_to_sizeof.at(data_type);
}

size_t sizeofSample(const SamplingType& sampling_type, const DataType& data_type) {
  static std::unordered_map<SamplingType, size_t> group_st_to_nels{{SamplingType::RF, 1},
                                                                   {SamplingType::IQ, 2}};

  return group_st_to_nels.at(sampling_type) * sizeofDataType(data_type);
}

}  // namespace urx::utils::group_helper
