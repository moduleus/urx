#include <catch2/catch_test_macros.hpp>

#include <urx/utils/probe_helper.h>

namespace urx::utils::probe_helper::test {

TEST_CASE("Update linear probe elements geometry", "[probe][helper]") {
  Probe probe;
  updateLinearElementsRectGeometry(probe, 4, {2.8, 1.6});

  REQUIRE(probe.element_geometries.size() == 4);

  ElementGeometry expected_geom{
      {{-1.4, -0.8, 0.0}, {1.4, -0.8, 0.0}, {1.4, 0.8, 0.0}, {-1.4, 0.8, 0.0}}};
  for (const auto& el_geom : probe.element_geometries) REQUIRE(*el_geom == expected_geom);
}

TEST_CASE("Update matrix probe elements geometry", "[probe][helper]") {
  Probe probe;
  updateMatrixElementsRectGeometry(probe, {4, 2}, {2.8, 1.6});

  REQUIRE(probe.element_geometries.size() == 8);

  ElementGeometry expected_geom{
      {{-1.4, -0.8, 0.0}, {1.4, -0.8, 0.0}, {1.4, 0.8, 0.0}, {-1.4, 0.8, 0.0}}};
  for (const auto& el_geom : probe.element_geometries) REQUIRE(*el_geom == expected_geom);
}

TEST_CASE("Update rca probe elements geometry", "[probe][helper]") {
  Probe probe;
  updateRcaElementsRectGeometry(probe, {4, 2}, {2.8, 1.6}, {1.2, 3.4});

  REQUIRE(probe.element_geometries.size() == 6);

  ElementGeometry expected_col_geom{
      {{-1.4, -0.8, 0.0}, {1.4, -0.8, 0.0}, {1.4, 0.8, 0.0}, {-1.4, 0.8, 0.0}}};
  ElementGeometry expected_row_geom{
      {{-0.6, -1.7, 0.0}, {0.6, -1.7, 0.0}, {0.6, 1.7, 0.0}, {-0.6, 1.7, 0.0}}};

  for (size_t i = 0; i < 4; i++) REQUIRE(*probe.element_geometries[i] == expected_col_geom);
  for (size_t i = 4; i < 6; i++) REQUIRE(*probe.element_geometries[i] == expected_row_geom);
}

}  // namespace urx::utils::probe_helper::test
