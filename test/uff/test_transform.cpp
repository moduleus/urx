#include <uff/transform.h>
#include <uff/vector.h>

#include <catch2/catch_all.hpp>

namespace uff::test {

TEST_CASE("Transform CTOR", "[test_transform]") {
  const Transform t_1;
  const Vector3D<double> c_1 = Vector3D<double>{0, 0, 0};
  const Transform t_2 = {c_1, c_1};

  REQUIRE(t_1 == t_2);
  REQUIRE(t_1.rotation == c_1);
  REQUIRE(t_1.translation == c_1);
}

TEST_CASE("Transform::operator==", "[test_transform]") {
  Transform t_1;
  Transform t_2;

  Vector3D<double> r_1 = t_1.rotation;
  Vector3D<double> r_2 = t_2.rotation;

  REQUIRE(r_1 == r_2);
  REQUIRE(t_1 == t_2);

  r_1 = Vector3D<double>(42, 0, 5);
  REQUIRE(r_1 != r_2);
  REQUIRE(t_1 == t_2);

  t_1.rotation = r_1;
  REQUIRE(t_1 != t_2);

  r_2 = Vector3D<double>(42, 0, 5);
  REQUIRE(r_1 == r_2);
  REQUIRE(t_1 != t_2);

  t_2.rotation = r_2;
  REQUIRE(t_1 == t_2);
}

}  // namespace uff::test
