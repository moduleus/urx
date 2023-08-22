#include <uff/time_offset_base.h>
#include <catch2/catch.hpp>

namespace uff::test {

TEST_CASE("TimeOffsetBase CTOR", "[test_time_offset_base]") {
  const TimeOffsetBase tob_1;
  REQUIRE(tob_1.timeOffset() == 0.);

  TimeOffsetBase tob_2 = TimeOffsetBase(0);
  REQUIRE(tob_1 == tob_2);

  tob_2 = TimeOffsetBase(42);
  REQUIRE(tob_1 != tob_2);
}

}  // namespace uff::test
