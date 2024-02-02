#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include <urx/group.h>
#include <urx/utils/group_helper.h>

#ifdef WITH_PYTHON
#include <pybind11/pybind11.h>
#else
#include <stdexcept>
#endif

#define UNUSED(expr) \
  do {               \
    (void)(expr);    \
  } while (0)

namespace urx::utils::GroupHelper {

size_t sizeof_data_type(const DataType& data_type) {
  static std::unordered_map<DataType, size_t> group_dt_to_sizeof{
      {DataType::INT16, sizeof(int16_t)},
      {DataType::INT32, sizeof(int32_t)},
      {DataType::FLOAT, sizeof(float)},
      {DataType::DOUBLE, sizeof(double)}};

  return group_dt_to_sizeof.at(data_type);
}

size_t sizeof_sample(const SamplingType& sampling_type, const DataType& data_type) {
  static std::unordered_map<SamplingType, size_t> group_st_to_nels{
      {SamplingType::RF, 1}, {SamplingType::IQ, 2}};

  return group_st_to_nels.at(sampling_type) * sizeof_data_type(data_type);
}

std::string py_get_format(const DataType& data_type) {
#ifdef WITH_PYTHON
  static std::unordered_map<DataType, std::string> group_dt_to_sizeof{
      {DataType::INT16, pybind11::format_descriptor<int16_t>::format()},
      {DataType::INT32, pybind11::format_descriptor<int32_t>::format()},
      {DataType::FLOAT, pybind11::format_descriptor<float>::format()},
      {DataType::DOUBLE, pybind11::format_descriptor<double>::format()}};

  return group_dt_to_sizeof.at(data_type);
#else
  UNUSED(data_type);
  throw std::runtime_error("Python not supported.");
#endif
}

}  // namespace urx::utils::GroupHelper
