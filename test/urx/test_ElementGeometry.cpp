#include <memory>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/element_geometry.h>
#include <urx/vector.h>

namespace urx::test {

TEST_CASE("ElementGeometry CTOR & operator==", "[test_transform]") {
  ElementGeometry eg_1;
  const Vector3D<double> v_1;
  const Vector3D<double> v_2{1, 2, 3.14};
  const ElementGeometry eg_2 = ElementGeometry({v_1, v_2});

  REQUIRE(eg_1 != eg_2);
  eg_1.perimeter = {v_1, v_2};
  REQUIRE(eg_1 == eg_2);
  REQUIRE(eg_1.perimeter == eg_2.perimeter);
}

}  // namespace urx::test
