#include <cstdio>
#include <utility>

#include <urx/utils/common.h>

namespace urx::utils {

std::string format_index_with_leading_zeros(const std::size_t index, const int length) {
  std::string str;
  str.resize(snprintf(nullptr, 0, "%0*zu", length, index) + 1);
  snprintf(str.data(), str.size(), "%0*zu", length, index);
  str.pop_back();
  return str;
}

}  // namespace urx::utils