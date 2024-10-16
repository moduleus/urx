#include <catch2/catch_test_macros.hpp>

#include <urx/urx.h>
#include <urx/version.h>

namespace urx::test {

TEST_CASE("Version CTOR", "[test_version]") {
  const Version v_1;
  REQUIRE(v_1.major == URX_VERSION_MAJOR);
  REQUIRE(v_1.minor == URX_VERSION_MINOR);
  REQUIRE(v_1.patch == URX_VERSION_PATCH);

  Version v_2 = Version{URX_VERSION_MAJOR, URX_VERSION_MINOR, URX_VERSION_PATCH};
  REQUIRE(v_1 == v_2);

  v_2 = Version{0, 42, 100};
  REQUIRE(v_1 != v_2);
}

}  // namespace urx::test
