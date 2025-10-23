#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/utils/common.h>

namespace urx::utils::test {

TEST_CASE("formatIndexWithLeadingZeros", "[Common]") {
  for (size_t i = 0; i < 100000; ++i) {
    std::string i_str = std::to_string(i);

    std::string i_formated = formatIndexWithLeadingZeros(i, 4);

    if (i_str.size() >= 4) {
      REQUIRE(i_str == i_formated);
    } else {
      for (size_t j = 0; j < i_formated.size(); ++j) {
        if (j < 4 - i_str.size()) {
          REQUIRE(i_formated.at(j) == '0');
        } else {
          REQUIRE(i_formated.at(j) == i_str.at(j - (4 - i_str.size())));
        }
      }
      for (size_t j = 4 - i_str.size(); j < i_str.size(); ++j) {
      }
    }
  }
}

TEST_CASE("getElementIndex double", "[Common]") {
  std::vector<double> vec(10000);
  std::iota(std::begin(vec), std::end(vec), 0);
  for (size_t i = -vec.size(); i < 2 * vec.size(); ++i) {
    if (i >= 0 && i < vec.size()) {
      REQUIRE(i == common::getElementIndex(vec, vec.at(i), true));
    } else {
      REQUIRE(std::nullopt == common::getElementIndex(vec, vec.at(i), true));
    }
  }
}

}  // namespace urx::utils::test