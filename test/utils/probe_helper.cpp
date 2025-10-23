#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/probe.h>
#include <urx/utils/probe_helper.h>
#include <urx/vector.h>

namespace urx::utils::probe_helper::test {

TEST_CASE("Update linear probe elements geometry", "[probe][helper]") {
  Probe probe;
  probe.elements.resize(4);

  updateLinearElementsRectGeometry(probe, {2.8, 1.6});

  REQUIRE(probe.element_geometries.size() == 1);

  auto sh_el_geom = probe.element_geometries[0];

  ElementGeometry expected_geom{
      {{-1.4, -0.8, 0.0}, {1.4, -0.8, 0.0}, {1.4, 0.8, 0.0}, {-1.4, 0.8, 0.0}}};
  REQUIRE(*sh_el_geom == expected_geom);

  for (const auto& element : probe.elements) {
    REQUIRE(*(element.element_geometry.lock()) == *sh_el_geom);
    REQUIRE(element.element_geometry.lock().get() == sh_el_geom.get());
  }
}

TEST_CASE("Update matrix probe elements geometry", "[probe][helper]") {
  Probe probe;
  probe.elements.resize(8);

  updateMatrixElementsRectGeometry(probe, {2.8, 1.6});

  REQUIRE(probe.element_geometries.size() == 1);

  auto sh_el_geom = probe.element_geometries[0];

  ElementGeometry expected_geom{
      {{-1.4, -0.8, 0.0}, {1.4, -0.8, 0.0}, {1.4, 0.8, 0.0}, {-1.4, 0.8, 0.0}}};
  REQUIRE(*sh_el_geom == expected_geom);

  for (const auto& element : probe.elements) {
    REQUIRE(*(element.element_geometry.lock()) == *sh_el_geom);
    REQUIRE(element.element_geometry.lock().get() == sh_el_geom.get());
  }
}

TEST_CASE("Update rca probe elements geometry", "[probe][helper]") {
  Probe probe;
  probe.elements.resize(6);

  updateRcaElementsRectGeometry(probe, {4, 2}, {2.8, 1.6}, {1.2, 3.4});

  REQUIRE(probe.element_geometries.size() == 2);

  ElementGeometry expected_col_geom{
      {{-1.4, -0.8, 0.0}, {1.4, -0.8, 0.0}, {1.4, 0.8, 0.0}, {-1.4, 0.8, 0.0}}};
  ElementGeometry expected_row_geom{
      {{-0.6, -1.7, 0.0}, {0.6, -1.7, 0.0}, {0.6, 1.7, 0.0}, {-0.6, 1.7, 0.0}}};

  auto sh_col_el_geom = probe.element_geometries[0];
  auto sh_row_el_geom = probe.element_geometries[1];

  REQUIRE(*sh_col_el_geom == expected_col_geom);
  REQUIRE(*sh_row_el_geom == expected_row_geom);

  for (size_t i = 0; i < 4; i++) {
    REQUIRE(*(probe.elements[i].element_geometry.lock()) == *sh_col_el_geom);
    REQUIRE(probe.elements[i].element_geometry.lock().get() == sh_col_el_geom.get());
  }

  for (size_t i = 4; i < 6; i++) {
    REQUIRE(*(probe.elements[i].element_geometry.lock()) == *sh_row_el_geom);
    REQUIRE(probe.elements[i].element_geometry.lock().get() == sh_row_el_geom.get());
  }
}

TEST_CASE("Update rca probe elements geometry with inconsistent nb_elements throw exception",
          "[probe][helper]") {
  Probe probe;
  probe.elements.resize(6);
  REQUIRE_THROWS(updateRcaElementsRectGeometry(probe, {3, 2}, {2.8, 1.6}, {1.2, 3.4}));
}

}  // namespace urx::utils::probe_helper::test
