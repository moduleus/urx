#include <catch2/catch_test_macros.hpp>

#include <uff/uff.h>
#include <uff/version.h>

namespace uff::test {

TEST_CASE("Version CTOR", "[test_version]") {
  const Version v_1;
  REQUIRE(v_1.major == UFF_VERSION_MAJOR);
  REQUIRE(v_1.minor == UFF_VERSION_MINOR);
  REQUIRE(v_1.patch == UFF_VERSION_PATCH);

  Version v_2 = Version{UFF_VERSION_MAJOR, UFF_VERSION_MINOR, UFF_VERSION_PATCH};
  REQUIRE(v_1 == v_2);

  v_2 = Version{0, 42, 100};
  REQUIRE(v_1 != v_2);
}

}  // namespace uff::test
