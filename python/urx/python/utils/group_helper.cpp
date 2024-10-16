#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include <pybind11/pybind11.h>

#include <urx/enums.h>
#include <urx/python/utils/group_helper.h>

namespace urx::python::utils {

std::string pyGetFormat(const DataType& data_type) {
  static std::unordered_map<DataType, std::string> group_dt_to_sizeof{
      {DataType::INT16, pybind11::format_descriptor<int16_t>::format()},
      {DataType::INT32, pybind11::format_descriptor<int32_t>::format()},
      {DataType::FLOAT, pybind11::format_descriptor<float>::format()},
      {DataType::DOUBLE, pybind11::format_descriptor<double>::format()}};

  return group_dt_to_sizeof.at(data_type);
}

}  // namespace urx::python::utils
