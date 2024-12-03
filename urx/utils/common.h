#pragma once

#include <string>
#include <vector>
#include <version>

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

/// Return the first index corresponding to the target element in the element vector
/// If the element does not belong to the vector then -1 is returned
template <typename T>
int32_t getEltId(const std::vector<T> &elt_vec, const T &elt) {
  const ptrdiff_t res_id =
      std::distance(elt_vec.cbegin(), std::find(elt_vec.cbegin(), elt_vec.cend(), elt));
  if (res_id < 0 || static_cast<size_t>(res_id) >= elt_vec.size()) {
    return -1;
  }
  return res_id;
};

}  // namespace urx::utils
