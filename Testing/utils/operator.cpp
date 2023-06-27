#include <iostream>
#include <sstream>

#include <catch2/catch.hpp>

#include <uff/dataset.h>

namespace uff::test {

std::ostream& operator<<(std::ostream& os, const uff::Dataset<short>& d) {
  os << "toto";
  return os;
}

TEST_CASE("<< Operator", "[operator]") {
  uff::Dataset<short> dataset;
  std::stringstream ss;
  ss << "toto";

  std::stringstream ss_2;
  ss_2 << dataset;

  REQUIRE(ss.str() == ss_2.str());
}

}  // namespace uff::test
