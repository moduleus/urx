#include <memory>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <urx/detail/double_nan.h>

namespace urx::test {

TEST_CASE("DoubleNan", "[test_double_nan]") {
  DoubleNan dn_1;
  DoubleNan dn_2;
  DoubleNan out_dn;
  double out_d;

  // NaN
  REQUIRE(dn_1 == dn_2);
  REQUIRE(!(dn_1 != dn_2));

  constexpr double lrel = 0.00000000001;
  constexpr double labs = 0.00000000001;

  dn_1 = 1.2;
  dn_2 = 0.2;
  out_dn = dn_1 + dn_2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(1.4, lrel) && Catch::Matchers::WithinAbs(1.4, labs));
  out_dn = dn_1 - dn_2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(1.0, lrel) && Catch::Matchers::WithinAbs(1.0, labs));
  out_dn = dn_1 / dn_2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(6.0, lrel) && Catch::Matchers::WithinAbs(6.0, labs));
  out_dn = dn_1 * dn_2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(0.24, lrel) && Catch::Matchers::WithinAbs(0.24, labs));
  out_dn = dn_1 + 1.2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(2.4, lrel) && Catch::Matchers::WithinAbs(2.4, labs));
  out_dn = dn_1 - 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(0, lrel) && Catch::Matchers::WithinAbs(0, labs));
  out_dn = dn_1 / 1.2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(1.0, lrel) && Catch::Matchers::WithinAbs(1.0, labs));
  out_dn = dn_1 * 1.2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(1.44, lrel) && Catch::Matchers::WithinAbs(1.44, labs));
  out_dn = 1.2 + dn_2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(1.4, lrel) && Catch::Matchers::WithinAbs(1.4, labs));
  out_dn = 1.2 - dn_2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(1.0, lrel) && Catch::Matchers::WithinAbs(1.0, labs));
  out_dn = 1.2 / dn_2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(6, lrel) && Catch::Matchers::WithinAbs(6, labs));
  out_dn = 1.2 * dn_2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(0.24, lrel) && Catch::Matchers::WithinAbs(0.24, labs));

  out_d = dn_1 + dn_2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(1.4, lrel) && Catch::Matchers::WithinAbs(1.4, labs));
  out_d = dn_1 - dn_2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(1.0, lrel) && Catch::Matchers::WithinAbs(1.0, labs));
  out_d = dn_1 / dn_2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(6.0, lrel) && Catch::Matchers::WithinAbs(6.0, labs));
  out_d = dn_1 * dn_2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(0.24, lrel) && Catch::Matchers::WithinAbs(0.24, labs));
  out_d = dn_1 + 1.2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(2.4, lrel) && Catch::Matchers::WithinAbs(2.4, labs));
  out_d = dn_1 - 1.2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(0, lrel) && Catch::Matchers::WithinAbs(0, labs));
  out_d = dn_1 / 1.2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(1.0, lrel) && Catch::Matchers::WithinAbs(1.0, labs));
  out_d = dn_1 * 1.2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(1.44, lrel) && Catch::Matchers::WithinAbs(1.44, labs));
  out_d = 1.2 + dn_2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(1.4, lrel) && Catch::Matchers::WithinAbs(1.4, labs));
  out_d = 1.2 - dn_2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(1.0, lrel) && Catch::Matchers::WithinAbs(1.0, labs));
  out_d = 1.2 / dn_2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(6, lrel) && Catch::Matchers::WithinAbs(6, labs));
  out_d = 1.2 * dn_2;
  REQUIRE_THAT(out_d,
               Catch::Matchers::WithinRel(0.24, lrel) && Catch::Matchers::WithinAbs(0.24, labs));

  out_dn = 1.2;
  out_dn += 1.2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(2.4, lrel) && Catch::Matchers::WithinAbs(2.4, labs));
  out_dn = 1.2;
  out_dn -= 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(0, lrel) && Catch::Matchers::WithinAbs(0, labs));
  out_dn = 1.2;
  out_dn /= 1.2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(1.0, lrel) && Catch::Matchers::WithinAbs(1.0, labs));
  out_dn = 1.2;
  out_dn *= 1.2;
  REQUIRE_THAT(out_dn,
               Catch::Matchers::WithinRel(1.44, lrel) && Catch::Matchers::WithinAbs(1.44, labs));
}

}  // namespace urx::test
