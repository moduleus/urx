#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <typeindex>

namespace urx::utils::io {

#define DIFF_PTR(parent, child) \
  (reinterpret_cast<size_t>(&(child)) - reinterpret_cast<size_t>(&(parent)))

struct SerializeHelper {
  static std::map<std::type_index, std::map<size_t, std::string>> member_name;
};

}  // namespace urx::utils::io
