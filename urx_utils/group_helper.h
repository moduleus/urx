#pragma once

#include <cstddef>
#include <string>

#include <urx/group.h>

namespace urx {

class GroupHelper {
 public:
  GroupHelper() = default;

  GroupHelper(GroupHelper&& other) noexcept = delete;
  GroupHelper(GroupHelper const& other) = delete;
  GroupHelper& operator=(GroupHelper&& other) noexcept = delete;
  GroupHelper& operator=(GroupHelper const& other) = delete;

  static size_t sizeof_data_type(const Group::DataType& data_type);
  static std::string py_get_format(const Group::DataType& data_type);
};

}  // namespace urx
