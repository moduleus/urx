#pragma once

#include <cstddef>
#include <string>

#include <uff/group.h>

namespace uff {

class GroupHelper {
 public:
  explicit GroupHelper(const Group& group);

  GroupHelper(GroupHelper&& other) noexcept = delete;
  GroupHelper(GroupHelper const& other) = delete;
  GroupHelper& operator=(GroupHelper&& other) noexcept = delete;
  GroupHelper& operator=(GroupHelper const& other) = delete;

  size_t sizeof_data_type();
  std::string py_get_format();

 private:
  const Group& group_;
};

}  // namespace uff
