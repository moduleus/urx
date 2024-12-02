#include "common.h"

#include <cstdio>
#include <utility>

namespace urx::utils {

std::string formatIndexWithLeadingZeros(const std::size_t index, const int length) {
  std::string str;
  str.resize(snprintf(nullptr, 0, "%0*zu", length, index) + 1);
  snprintf(str.data(), str.size(), "%0*zu", length, index);
  str.pop_back();
  return str;
}

}  // namespace urx::utils