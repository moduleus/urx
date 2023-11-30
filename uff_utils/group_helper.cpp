#include "group_helper.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

#include <uff/group.h>

#ifdef WITH_PYTHON
#include <pybind11/pybind11.h>
#endif

namespace uff {

GroupHelper::GroupHelper(const Group& group) : group_(group) {}

size_t GroupHelper::sizeof_data_type() {
  static std::unordered_map<Group::DataType, size_t> group_dt_to_sizeof{
      {Group::DataType::INT16, sizeof(int16_t)},
      {Group::DataType::INT32, sizeof(int32_t)},
      {Group::DataType::FLOAT, sizeof(float)},
      {Group::DataType::DOUBLE, sizeof(double)}};

  return group_dt_to_sizeof.at(group_.data_type);
}

std::string GroupHelper::py_get_format() {
#ifdef WITH_PYTHON
  static std::unordered_map<Group::DataType, std::string> group_dt_to_sizeof{
      {Group::DataType::INT16, pybind11::format_descriptor<int16_t>::format()},
      {Group::DataType::INT32, pybind11::format_descriptor<int32_t>::format()},
      {Group::DataType::FLOAT, pybind11::format_descriptor<float>::format()},
      {Group::DataType::DOUBLE, pybind11::format_descriptor<double>::format()}};

  return group_dt_to_sizeof.at(group_.data_type);
#else
  throw std::runtime_exception("Python not supported.") :
#endif
}

}  // namespace uff
