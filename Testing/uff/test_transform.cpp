#include <uff/coordinates.h>
#include <uff/transform.h>
#include <uff/uff.h>
#include <catch2/catch.hpp>

namespace uff::test {

TEST_CASE("Transform CTOR", "[test_transform]") {
  const Transform t_1;
  const Coordinates3D<MetadataType> c_1 = Coordinates3D<MetadataType>{0, 0, 0};
  const Transform t_2 = {c_1, c_1};

  REQUIRE(t_1 == t_2);
  REQUIRE(t_1.rotation() == c_1);
  REQUIRE(t_1.translation() == c_1);
}

TEST_CASE("Transform::operator==", "[test_transform]") {
  Transform t_1;
  Transform t_2;

  Coordinates3D<MetadataType> r_1 = t_1.rotation();
  Coordinates3D<MetadataType> r_2 = t_2.rotation();

  REQUIRE(r_1 == r_2);
  REQUIRE(t_1 == t_2);

  r_1 = Coordinates3D<MetadataType>(42, 0, 5);
  REQUIRE(r_1 != r_2);
  REQUIRE(t_1 == t_2);

  t_1.setRotation(r_1);
  REQUIRE(t_1 != t_2);

  r_2 = Coordinates3D<MetadataType>(42, 0, 5);
  REQUIRE(r_1 == r_2);
  REQUIRE(t_1 != t_2);

  t_2.setRotation(r_2);
  REQUIRE(t_1 == t_2);
}

}  // namespace uff::test
