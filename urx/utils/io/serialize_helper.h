#pragma once

#include <cstddef>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace urx::utils::io {

#define DIFF_PTR(parent, child) \
  (reinterpret_cast<size_t>(&(child)) - reinterpret_cast<size_t>(&(parent)))

struct SerializeHelper {
  static std::unordered_map<std::type_index, std::unordered_map<size_t, std::string>> member_name;
};

}  // namespace urx::utils::io
