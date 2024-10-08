#include <limits>
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
  double out_f;

  // NaN
  REQUIRE(dn_1 == dn_2);
  REQUIRE(!(dn_1 != dn_2));

  constexpr double eps_rel_d = 0.00000000001;
  constexpr double eps_abs_d = 0.00000000001;

  constexpr float eps_rel_f = std::numeric_limits<float>::epsilon();
  constexpr float eps_abs_f = std::numeric_limits<float>::epsilon();

  dn_1 = 1.2;
  dn_2 = 0.2;
  out_dn = dn_1 + dn_2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.4, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.4, eps_abs_d));
  out_dn = dn_1 - dn_2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_dn = dn_1 / dn_2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(6.0, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(6.0, eps_abs_d));
  out_dn = dn_1 * dn_2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(0.24, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(0.24, eps_abs_d));
  out_dn = dn_1 + 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(2.4, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(2.4, eps_abs_d));
  out_dn = dn_1 - 1.2;
  REQUIRE_THAT(
      out_dn, Catch::Matchers::WithinRel(0, eps_rel_d) && Catch::Matchers::WithinAbs(0, eps_abs_d));
  out_dn = dn_1 / 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_dn = dn_1 * 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.44, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.44, eps_abs_d));
  out_dn = 1.2 + dn_2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.4, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.4, eps_abs_d));
  out_dn = 1.2 - dn_2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_dn = 1.2 / dn_2;
  REQUIRE_THAT(
      out_dn, Catch::Matchers::WithinRel(6, eps_rel_d) && Catch::Matchers::WithinAbs(6, eps_abs_d));
  out_dn = 1.2 * dn_2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(0.24, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(0.24, eps_abs_d));

  out_d = dn_1 + dn_2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(1.4, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.4, eps_abs_d));
  out_d = dn_1 - dn_2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_d = dn_1 / dn_2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(6.0, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(6.0, eps_abs_d));
  out_d = dn_1 * dn_2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(0.24, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(0.24, eps_abs_d));
  out_d = dn_1 + 1.2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(2.4, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(2.4, eps_abs_d));
  out_d = dn_1 - 1.2;
  REQUIRE_THAT(
      out_d, Catch::Matchers::WithinRel(0, eps_rel_d) && Catch::Matchers::WithinAbs(0, eps_abs_d));
  out_d = dn_1 / 1.2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_d = dn_1 * 1.2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(1.44, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.44, eps_abs_d));
  out_d = 1.2 + dn_2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(1.4, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.4, eps_abs_d));
  out_d = 1.2 - dn_2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_d = 1.2 / dn_2;
  REQUIRE_THAT(
      out_d, Catch::Matchers::WithinRel(6, eps_rel_d) && Catch::Matchers::WithinAbs(6, eps_abs_d));
  out_d = 1.2 * dn_2;
  REQUIRE_THAT(out_d, Catch::Matchers::WithinRel(0.24, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(0.24, eps_abs_d));

  out_f = dn_1 + dn_2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(1.4, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.4, eps_abs_d));
  out_f = dn_1 - dn_2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_f = dn_1 / dn_2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(6.0, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(6.0, eps_abs_d));
  out_f = dn_1 * dn_2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(0.24, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(0.24, eps_abs_d));
  out_f = dn_1 + 1.2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(2.4, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(2.4, eps_abs_d));
  out_f = dn_1 - 1.2;
  REQUIRE_THAT(
      out_f, Catch::Matchers::WithinRel(0, eps_rel_d) && Catch::Matchers::WithinAbs(0, eps_abs_d));
  out_f = dn_1 / 1.2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_f = dn_1 * 1.2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(1.44, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.44, eps_abs_d));
  out_f = 1.2 + dn_2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(1.4, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.4, eps_abs_d));
  out_f = 1.2 - dn_2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_f = 1.2 / dn_2;
  REQUIRE_THAT(
      out_f, Catch::Matchers::WithinRel(6, eps_rel_d) && Catch::Matchers::WithinAbs(6, eps_abs_d));
  out_f = 1.2 * dn_2;
  REQUIRE_THAT(out_f, Catch::Matchers::WithinRel(0.24, eps_rel_d) &&
                          Catch::Matchers::WithinAbs(0.24, eps_abs_d));

  out_dn = 1.2;
  out_dn += 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(2.4, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(2.4, eps_abs_d));
  out_dn = 1.2;
  out_dn = out_dn + 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(2.4, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(2.4, eps_abs_d));
  out_dn = 1.2;
  out_dn = 1.2 + out_dn;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(2.4, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(2.4, eps_abs_d));
  out_dn = 1.2;
  out_dn -= 1.2;
  REQUIRE_THAT(
      out_dn, Catch::Matchers::WithinRel(0, eps_rel_d) && Catch::Matchers::WithinAbs(0, eps_abs_d));
  out_dn = 1.2;
  out_dn = out_dn - 1.2;
  REQUIRE_THAT(
      out_dn, Catch::Matchers::WithinRel(0, eps_rel_d) && Catch::Matchers::WithinAbs(0, eps_abs_d));
  out_dn = 1.2;
  out_dn = 1.2 - out_dn;
  REQUIRE_THAT(
      out_dn, Catch::Matchers::WithinRel(0, eps_rel_d) && Catch::Matchers::WithinAbs(0, eps_abs_d));
  out_dn = 1.2;
  out_dn /= 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_dn = 1.2;
  out_dn = out_dn / 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_dn = 1.2;
  out_dn = 1.2 / out_dn;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.0, eps_abs_d));
  out_dn = 1.2;
  out_dn *= 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.44, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.44, eps_abs_d));
  out_dn = 1.2;
  out_dn = out_dn * 1.2;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.44, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.44, eps_abs_d));
  out_dn = 1.2;
  out_dn = 1.2 * out_dn;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.44, eps_rel_d) &&
                           Catch::Matchers::WithinAbs(1.44, eps_abs_d));

  out_dn = 1.2f;
  out_dn += 1.2f;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(2.4f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(2.4f, eps_abs_f));
  out_dn = 1.2f;
  out_dn = out_dn + 1.2f;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(2.4f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(2.4f, eps_abs_f));
  out_dn = 1.2f;
  out_dn = 1.2f + out_dn;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(2.4f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(2.4f, eps_abs_f));
  out_dn = 1.2f;
  out_dn -= 1.2f;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(0.f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(0.f, eps_abs_f));
  out_dn = 1.2f;
  out_dn = out_dn - 1.2f;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(0.f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(0.f, eps_abs_f));
  out_dn = 1.2f;
  out_dn = 1.2f - out_dn;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(0.f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(0.f, eps_abs_f));
  out_dn = 1.2f;
  out_dn /= 1.2f;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(1.0f, eps_abs_f));
  out_dn = 1.2f;
  out_dn = out_dn / 1.2f;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(1.0f, eps_abs_f));
  out_dn = 1.2f;
  out_dn = 1.2f / out_dn;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.0f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(1.0f, eps_abs_f));
  out_dn = 1.2f;
  out_dn *= 1.2f;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.44f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(1.44f, eps_abs_f));
  out_dn = 1.2f;
  out_dn = out_dn * 1.2f;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.44f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(1.44f, eps_abs_f));
  out_dn = 1.2f;
  out_dn = 1.2f * out_dn;
  REQUIRE_THAT(out_dn, Catch::Matchers::WithinRel(1.44f, eps_rel_f) &&
                           Catch::Matchers::WithinAbs(1.44f, eps_abs_f));

  out_dn = 12;
  out_dn += 12;
  REQUIRE(out_dn == DoubleNan(24));
  out_dn = 12;
  out_dn = out_dn + 12;
  REQUIRE(out_dn == DoubleNan(24));
  out_dn = 12;
  out_dn = 12 + out_dn;
  REQUIRE(out_dn == DoubleNan(24));
  out_dn = 12;
  out_dn -= 12;
  REQUIRE(out_dn == DoubleNan(0));
  out_dn = 12;
  out_dn = out_dn - 12;
  REQUIRE(out_dn == DoubleNan(0));
  out_dn = 12;
  out_dn = 12 - out_dn;
  REQUIRE(out_dn == DoubleNan(0));
  out_dn = 12;
  out_dn /= 12;
  REQUIRE(out_dn == DoubleNan(1));
  out_dn = 12;
  out_dn = out_dn / 12;
  REQUIRE(out_dn == DoubleNan(1));
  out_dn = 12;
  out_dn = 12 / out_dn;
  REQUIRE(out_dn == DoubleNan(1));
  out_dn = 12;
  out_dn *= 12;
  REQUIRE(out_dn == DoubleNan(144));
  out_dn = 12;
  out_dn = out_dn * 12;
  REQUIRE(out_dn == DoubleNan(144));
  out_dn = 12;
  out_dn = 12 * out_dn;
  REQUIRE(out_dn == DoubleNan(144));

  out_dn = 12ull;
  out_dn += 12ull;
  REQUIRE(out_dn == DoubleNan(24ull));
  out_dn = 12ull;
  out_dn = out_dn + 12ull;
  REQUIRE(out_dn == DoubleNan(24ull));
  out_dn = 12ull;
  out_dn = 12ull + out_dn;
  REQUIRE(out_dn == DoubleNan(24ull));
  out_dn = 12ull;
  out_dn -= 12ull;
  REQUIRE(out_dn == DoubleNan(0ull));
  out_dn = 12ull;
  out_dn = out_dn - 12ull;
  REQUIRE(out_dn == DoubleNan(0ull));
  out_dn = 12ull;
  out_dn = 12ull - out_dn;
  REQUIRE(out_dn == DoubleNan(0ull));
  out_dn = 12ull;
  out_dn /= 12ull;
  REQUIRE(out_dn == DoubleNan(1ull));
  out_dn = 12ull;
  out_dn = out_dn / 12ull;
  REQUIRE(out_dn == DoubleNan(1ull));
  out_dn = 12ull;
  out_dn = 12ull / out_dn;
  REQUIRE(out_dn == DoubleNan(1ull));
  out_dn = 12ull;
  out_dn *= 12ull;
  REQUIRE(out_dn == DoubleNan(144ull));
  out_dn = 12ull;
  out_dn = out_dn * 12ull;
  REQUIRE(out_dn == DoubleNan(144ull));
  out_dn = 12ull;
  out_dn = 12ull * out_dn;
  REQUIRE(out_dn == DoubleNan(144ull));
}

}  // namespace urx::test
