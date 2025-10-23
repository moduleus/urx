#pragma once

#include <algorithm>  // IWYU pragma: keep
#include <cstddef>
#include <iterator>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>
#include <version>

#include <urx/detail/compare.h>
#include <urx/utils/cpp.h>
#include <urx/utils/export.h>

namespace urx::utils::common {

///
///\brief Index to string with leading zeros.
///
///\note Avoid using std::format function that is not yet implemented by all compilers claiming c++20 support.
///
///\param index The index value
///\param length The string output length (minimum)
///\return The index value as a string with as many necessary leading zeros to fill length characters
///
URX_UTILS_EXPORT std::string formatIndexWithLeadingZeros(std::size_t index, int length);

/// Alias to match std::find return type
template <typename T>
using FindResultType = decltype(std::find_if(typename std::vector<T>::const_iterator{},
                                             typename std::vector<T>::const_iterator{}, T{}));

template <typename T, typename U>
inline auto getElementItByValue(const std::vector<T> &elt_vec, const U &elt) {
  return std::find_if(elt_vec.cbegin(), elt_vec.cend(),
                      [elt](const T &elt_i) { return valueComparison(elt_i, elt); });
};

template <typename T, typename U>
inline std::enable_if_t<!IsSharedPtr<T>::value, FindResultType<T>> getElementIt(
    const std::vector<T> &elt_vec, const U &elt) {
  return std::find_if(elt_vec.cbegin(), elt_vec.cend(),
                      [elt](const T &elt_i) { return &elt_i == &elt; });
};

template <typename T, typename U>
inline std::enable_if_t<IsSharedPtr<T>::value && std::is_same_v<T, U>, FindResultType<T>>
getElementIt(const std::vector<T> &elt_vec, const U &elt) {
  return std::find_if(elt_vec.cbegin(), elt_vec.cend(),
                      [elt](const T &elt_i) { return elt_i.get() == elt.get(); });
};

template <typename T, typename U>
inline std::enable_if_t<IsSharedPtr<T>::value && IsWeakPtr<U>::value, FindResultType<T>>
getElementIt(const std::vector<T> &elt_vec, const U &elt) {
  return std::find_if(elt_vec.cbegin(), elt_vec.cend(), [elt](const T &elt_i) {
    return !elt.expired() && elt_i.get() == elt.lock().get();
  });
};

/// Return the first index corresponding to the target element in the element vector
/// If the element does not belong to the vector then -1 is returned
template <typename T, typename U>
std::optional<size_t> getElementIndex(const std::vector<T> &elt_vec, const U &elt,
                                      bool value_comparison) {
  auto it = value_comparison ? getElementItByValue(elt_vec, elt) : getElementIt(elt_vec, elt);
  if (it == elt_vec.end()) {
    return std::nullopt;
  }
  const ptrdiff_t res_id = std::distance(elt_vec.cbegin(), it);
  return res_id;
}

}  // namespace urx::utils::common
