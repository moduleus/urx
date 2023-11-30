#include <memory>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <urx/detail/double_nan.h>

namespace urx::test {

TEST_CASE("DoubleNan", "[test_double_nan]") {
  DoubleNan d1;
  DoubleNan d2;
  DoubleNan r1;
  double rr1;

  // NaN
  REQUIRE(d1 == d2);
  REQUIRE(!(d1 != d2));

  constexpr double lrel = 0.00000000001;
  constexpr double labs = 0.00000000001;

  d1 = 1.2;
  d2 = 0.2;
  r1 = d1 + d2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(1.4, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = d1 - d2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(1.0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = d1 / d2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(6.0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = d1 * d2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(0.24, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = d1 + 1.2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(2.4, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = d1 - 1.2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = d1 / 1.2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(1.0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = d1 * 1.2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(1.44, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = 1.2 + d2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(1.4, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = 1.2 - d2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(1.0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = 1.2 / d2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(6, lrel) || Catch::Matchers::WithinAbs(0, labs));
  r1 = 1.2 * d2;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(0.24, lrel) || Catch::Matchers::WithinAbs(0, labs));

  rr1 = d1 + d2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(1.4, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = d1 - d2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(1.0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = d1 / d2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(6.0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = d1 * d2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(0.24, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = d1 + 1.2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(2.4, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = d1 - 1.2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = d1 / 1.2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(1.0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = d1 * 1.2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(1.44, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = 1.2 + d2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(1.4, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = 1.2 - d2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(1.0, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = 1.2 / d2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(6, lrel) || Catch::Matchers::WithinAbs(0, labs));
  rr1 = 1.2 * d2;
  REQUIRE_THAT(rr1, Catch::Matchers::WithinRel(0.24, lrel) || Catch::Matchers::WithinAbs(0, labs));
}

}  // namespace urx::test
