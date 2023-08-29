#include <uff/vector.h>
#include <catch2/catch.hpp>

namespace uff::test {

TEST_CASE("Vector2D<int>::operator==", "[test_vector]") {
  Vector2D<int> v_1{0, 0};
  const Vector2D<int> v_2{0, 0};
  const Vector2D<int> v_3{5, -3};
  REQUIRE(v_1 == v_2);
  REQUIRE(v_1._x == v_2._x);
  REQUIRE(v_1._y == v_2._y);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y != v_3._y);

  v_1 = Vector2D<int>{5, -3};
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x != v_2._x);
  REQUIRE(v_1._y != v_2._y);
  REQUIRE(v_1 == v_3);
  REQUIRE(v_1._x == v_3._x);
  REQUIRE(v_1._y == v_3._y);

  v_1._x = 0.;
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x == v_2._x);
  REQUIRE(v_1._y != v_2._y);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y == v_3._y);

  v_1._y = 0.;
  REQUIRE(v_1 == v_2);
  REQUIRE(v_1._x == v_2._x);
  REQUIRE(v_1._y == v_2._y);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y != v_3._y);

  v_1 = v_3;
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x != v_2._x);
  REQUIRE(v_1._y != v_2._y);
  REQUIRE(v_1 == v_3);
  REQUIRE(v_1._x == v_3._x);
  REQUIRE(v_1._y == v_3._y);

  const Vector2D<int> v_4{42, -7};
  v_1 = v_4;
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x != v_2._x);
  REQUIRE(v_1._y != v_2._y);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y != v_3._y);
  REQUIRE(v_1._x == 42);
  REQUIRE(v_1._y == -7);
}

TEST_CASE("Vector3D<double>::operator==", "[test_vector]") {
  Vector3D<double> v_1{0., 0., 0.};
  const Vector3D<double> v_2{0., 0., 0.};
  const Vector3D<double> v_3{5., -3., 42.};
  REQUIRE(v_1 == v_2);
  REQUIRE(v_1._x == v_2._x);
  REQUIRE(v_1._y == v_2._y);
  REQUIRE(v_1._z == v_2._z);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y != v_3._y);
  REQUIRE(v_1._z != v_3._z);

  v_1 = Vector3D<double>{5., -3., 42.};
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x != v_2._x);
  REQUIRE(v_1._y != v_2._y);
  REQUIRE(v_1._z != v_2._z);
  REQUIRE(v_1 == v_3);
  REQUIRE(v_1._x == v_3._x);
  REQUIRE(v_1._y == v_3._y);
  REQUIRE(v_1._z == v_3._z);

  v_1._x = 0.;
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x == v_2._x);
  REQUIRE(v_1._y != v_2._y);
  REQUIRE(v_1._z != v_2._z);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y == v_3._y);
  REQUIRE(v_1._z == v_3._z);

  v_1._y = 0.;
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x == v_2._x);
  REQUIRE(v_1._y == v_2._y);
  REQUIRE(v_1._z != v_2._z);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y != v_3._y);
  REQUIRE(v_1._z == v_3._z);

  v_1._z = 0.;
  REQUIRE(v_1 == v_2);
  REQUIRE(v_1._x == v_2._x);
  REQUIRE(v_1._y == v_2._y);
  REQUIRE(v_1._z == v_2._z);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y != v_3._y);
  REQUIRE(v_1._z != v_3._z);

  v_1 = v_3;
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x != v_2._x);
  REQUIRE(v_1._y != v_2._y);
  REQUIRE(v_1._z != v_2._z);
  REQUIRE(v_1 == v_3);
  REQUIRE(v_1._x == v_3._x);
  REQUIRE(v_1._y == v_3._y);
  REQUIRE(v_1._z == v_3._z);

  const Vector3D<double> v_4{42., -7., 8.52};
  v_1 = v_4;
  REQUIRE(v_1 != v_2);
  REQUIRE(v_1._x != v_2._x);
  REQUIRE(v_1._y != v_2._y);
  REQUIRE(v_1._z != v_2._z);
  REQUIRE(v_1 != v_3);
  REQUIRE(v_1._x != v_3._x);
  REQUIRE(v_1._y != v_3._y);
  REQUIRE(v_1._z != v_3._z);
  REQUIRE(v_1._x == 42.);
  REQUIRE(v_1._y == -7.);
  REQUIRE(v_1._z == 8.52);
}

}  // namespace uff::test
