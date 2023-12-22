#pragma once

#include <numeric>
#include <string>
#include <vector>

namespace urx {

///
///\brief Index to string with leading zeros.
///
///\note Avoid using std::format function that is not yet implemented by all compilers claiming c++20 support.
///
///\param index The index value
///\param length The string output length (minimum)
///\return The index value as a string with as many necessary leading zeros to fill length characters
///
std::string format_index_with_leading_zeros(const std::size_t index, const int length);

///
///\brief Inverse the max index in map.
///
///\tparam T Index type
///\param map The map.
///\return std::vector<std::vector<T>> The max index referenced in the map.
///
template <class T>
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T> map_max(
    const std::vector<std::vector<T>>& map) {
  if (map.empty()) return 0;
  return std::accumulate(map.begin(), map.end(), 0,
                         [](uint32_t max, const auto& vec) {
                           if (vec.empty()) return max;
                           return std::max(max, *std::max_element(vec.begin(), vec.end()));
                         }) +
         1;
}

///
///\brief Inverse a 2D map.
///
///\tparam T Index type
///\param map The map to inverse
///\return std::vector<std::vector<T>> The inverted map.
///
template <class T>
std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, std::vector<std::vector<T>>>
map_invert(const std::vector<std::vector<T>>& map) {
  std::vector<std::vector<T>> inv_map(map_max(map));
  for (size_t i = 0; i < map.size(); ++i) {
    for (const T j : map[i]) inv_map[i].push_back(j);
  }

  return inv_map;
}

}  // namespace urx
