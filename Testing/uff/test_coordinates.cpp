#include <uff/coordinates.h>
#include <catch2/catch.hpp>

namespace uff::test {

TEST_CASE("Coordinates2D<int>::operator==", "[test_coordinates]") {
  Coordinates2D<int> c_1{0, 0};
  const Coordinates2D<int> c_2{0, 0};
  const Coordinates2D<int> c_3{5, -3};
  REQUIRE(c_1 == c_2);
  REQUIRE(c_1.x() == c_2.x());
  REQUIRE(c_1.y() == c_2.y());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() != c_3.y());

  c_1 = Coordinates2D<int>{5, -3};
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() != c_2.x());
  REQUIRE(c_1.y() != c_2.y());
  REQUIRE(c_1 == c_3);
  REQUIRE(c_1.x() == c_3.x());
  REQUIRE(c_1.y() == c_3.y());

  c_1.setX(0);
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() == c_2.x());
  REQUIRE(c_1.y() != c_2.y());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() == c_3.y());

  c_1.setY(0);
  REQUIRE(c_1 == c_2);
  REQUIRE(c_1.x() == c_2.x());
  REQUIRE(c_1.y() == c_2.y());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() != c_3.y());

  c_1 = c_3;
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() != c_2.x());
  REQUIRE(c_1.y() != c_2.y());
  REQUIRE(c_1 == c_3);
  REQUIRE(c_1.x() == c_3.x());
  REQUIRE(c_1.y() == c_3.y());

  const Coordinates2D<int> c_4{42, -7};
  c_1 = c_4;
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() != c_2.x());
  REQUIRE(c_1.y() != c_2.y());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() != c_3.y());
  REQUIRE(c_1.x() == 42);
  REQUIRE(c_1.y() == -7);
}

TEST_CASE("Coordinates3D<double>::operator==", "[test_coordinates]") {
  Coordinates3D<double> c_1{0., 0., 0.};
  const Coordinates3D<double> c_2{0., 0., 0.};
  const Coordinates3D<double> c_3{5., -3., 42.};
  REQUIRE(c_1 == c_2);
  REQUIRE(c_1.x() == c_2.x());
  REQUIRE(c_1.y() == c_2.y());
  REQUIRE(c_1.z() == c_2.z());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() != c_3.y());
  REQUIRE(c_1.z() != c_3.z());

  c_1 = Coordinates3D<double>{5., -3., 42.};
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() != c_2.x());
  REQUIRE(c_1.y() != c_2.y());
  REQUIRE(c_1.z() != c_2.z());
  REQUIRE(c_1 == c_3);
  REQUIRE(c_1.x() == c_3.x());
  REQUIRE(c_1.y() == c_3.y());
  REQUIRE(c_1.z() == c_3.z());

  c_1.setX(0.);
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() == c_2.x());
  REQUIRE(c_1.y() != c_2.y());
  REQUIRE(c_1.z() != c_2.z());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() == c_3.y());
  REQUIRE(c_1.z() == c_3.z());

  c_1.setY(0.);
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() == c_2.x());
  REQUIRE(c_1.y() == c_2.y());
  REQUIRE(c_1.z() != c_2.z());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() != c_3.y());
  REQUIRE(c_1.z() == c_3.z());

  c_1.setZ(0.);
  REQUIRE(c_1 == c_2);
  REQUIRE(c_1.x() == c_2.x());
  REQUIRE(c_1.y() == c_2.y());
  REQUIRE(c_1.z() == c_2.z());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() != c_3.y());
  REQUIRE(c_1.z() != c_3.z());

  c_1 = c_3;
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() != c_2.x());
  REQUIRE(c_1.y() != c_2.y());
  REQUIRE(c_1.z() != c_2.z());
  REQUIRE(c_1 == c_3);
  REQUIRE(c_1.x() == c_3.x());
  REQUIRE(c_1.y() == c_3.y());
  REQUIRE(c_1.z() == c_3.z());

  const Coordinates3D<double> c_4{42., -7., 8.52};
  c_1 = c_4;
  REQUIRE(c_1 != c_2);
  REQUIRE(c_1.x() != c_2.x());
  REQUIRE(c_1.y() != c_2.y());
  REQUIRE(c_1.z() != c_2.z());
  REQUIRE(c_1 != c_3);
  REQUIRE(c_1.x() != c_3.x());
  REQUIRE(c_1.y() != c_3.y());
  REQUIRE(c_1.z() != c_3.z());
  REQUIRE(c_1.x() == 42.);
  REQUIRE(c_1.y() == -7.);
  REQUIRE(c_1.z() == 8.52);
}

}  // namespace uff::test
