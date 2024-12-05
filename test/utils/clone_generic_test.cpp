#include <memory>

#include <catch2/catch_test_macros.hpp>
#include <test/utils/clone_generic_test.h>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx::utils::test {

TEST_CASE("Clone Vector2D", "[Clone]") {
  Vector2D<double> d_vec = {1.23, 4.56};
  generic_clone_test(d_vec);
}

TEST_CASE("Clone Vector3D", "[Clone]") {
  Vector3D<double> d_vec = {1.23, 4.56, -7.89};
  generic_clone_test(d_vec);
}

TEST_CASE("Clone Version", "[Clone]") {
  Version vers = {1, 2, 3};
  generic_clone_test(vers);
}

TEST_CASE("Clone Transform", "[Clone]") {
  Transform t = {{1, 2, 3}, {4, 5, 6}};
  generic_clone_test(t);
}

TEST_CASE("Clone Wave", "[Clone]") {
  Wave w;
  generic_clone_test(w);
  w.type = urx::WaveType::DIVERGING_WAVE;
  w.time_zero = -42.42;
  w.time_zero_reference_point = {1.23, 4.56, 7.89};
  w.parameters = {1.23, 4.56, 7.89, 10.11, 12.13};
  generic_clone_test(w);
}

TEST_CASE("Clone Element", "[Clone]") {
  Element e;
  generic_clone_test(e);

  std::shared_ptr<ElementGeometry> element_geometry = std::make_shared<ElementGeometry>();
  std::shared_ptr<ImpulseResponse> impulse_response = std::make_shared<ImpulseResponse>();

  e.transform = {{1.23, 4.56, 7.89}, {10, -11.1, 12.3}};
  e.element_geometry = element_geometry;
  e.impulse_response = impulse_response;

  generic_clone_test(e);
}

TEST_CASE("Clone ElementGeometry", "[Clone]") {
  ElementGeometry eg;
  generic_clone_test(eg);

  eg.perimeter = {{1.23, 4.56, 7.89}, {10, -11.1, 12.3}, {-1.23, -4.56, 7.89}, {-10, 11.1, -12.3}};

  generic_clone_test(eg);
}

TEST_CASE("Clone ImpulseResponse", "[Clone]") {
  ImpulseResponse ir;
  generic_clone_test(ir);

  ir.data = {1.23, 4.56, 7.89, 10.11, 12.13};
  ir.sampling_frequency = 120e6;
  ir.units = "Test";
  ir.time_offset = 10e3;

  generic_clone_test(ir);
}

}  // namespace urx::utils::test