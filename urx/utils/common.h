#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <version>

#include <urx/detail/compare.h>
#include <urx/utils/export.h>

namespace urx::utils {

///
///\brief Index to string with leading zeros.
///
///\note Avoid using std::format function that is not yet implemented by all compilers claiming c++20 support.
///
///\param index The index value
///\param length The string output length (minimum)
///\return The index value as a string with as many necessary leading zeros to fill length characters
///
URX_UTILS_EXPORT std::string formatIndexWithLeadingZeros(const std::size_t index, const int length);

template <typename T, typename U>
auto getElementIt(const std::vector<T> &elt_vec, const U &elt) {
  return std::find(elt_vec.begin(), elt_vec.end(), elt);
};

/// Return the first index corresponding to the target element in the element vector
/// If the element does not belong to the vector then -1 is returned
template <typename T, typename U>
int32_t getElementIndex(const std::vector<T> &elt_vec, const U &elt) {
  auto it = getElementIt(elt_vec, elt);
  if (it == elt_vec.end()) {
    return -1;
  }
  const ptrdiff_t res_id = std::distance(elt_vec.cbegin(), it);
  return res_id;
}

}  // namespace urx::utils
