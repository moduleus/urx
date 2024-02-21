#pragma once

#include <string>
#include <unordered_map>
#include <utility>

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
}  // namespace urx::utils::io::enums
