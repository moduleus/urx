#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include <H5Cpp.h>

#include <urx/enums.h>
#include <urx/utils/export.h>

namespace urx::utils::io::enums {
template <typename T, typename U>
std::unordered_map<U, T> invertMap(const std::unordered_map<T, U>& map) {
  std::unordered_map<U, T> retval;
  for (const auto& kv : map) {
    retval[kv.second] = kv.first;
  }
  return retval;
}

template <typename T>
T stringToEnumImpl(const std::string& enumeration, const std::unordered_map<std::string, T>& map) {
  auto search = map.find(enumeration);
  if (search != map.end()) {
    return map.at(enumeration);
  }
  return static_cast<T>(stoi(enumeration));
}

template <typename T>
std::string enumToStringImpl(T enumeration, const std::unordered_map<T, std::string>& map) {
  auto search = map.find(enumeration);
  if (search != map.end()) {
    return map.at(enumeration);
  }
  return std::to_string(static_cast<int>(enumeration));
}

template <typename T>
T stringToEnum(const std::string& enumeration);
template <typename T>
std::string enumToString(T enumeration);

[[deprecated("Don't expose HDF5 header.")]] URX_UTILS_EXPORT urx::DataType h5PredTypeToDataType(
    const H5::DataType& pred_type);
[[deprecated("Don't expose HDF5 header.")]] URX_UTILS_EXPORT const H5::PredType&
dataTypeToH5PredType(urx::DataType data_type);

}  // namespace urx::utils::io::enums
