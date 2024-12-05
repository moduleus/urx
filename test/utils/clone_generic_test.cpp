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
#include <urx/utils/test/dataset_gen.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx::utils::test {

TEST_CASE("Clone DoubleNan", "[Clone]") {
  DoubleNan d;
  generic_clone_test(d);
  d = 42.42;
  generic_clone_test(d);
}

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
  generic_clone_test(Wave());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t g_id = 0; g_id < d->acquisition.groups.size(); ++g_id) {
    auto& group = d->acquisition.groups.at(g_id);
    if (group != nullptr) {
      for (size_t e_id = 0; e_id < group->sequence.size(); ++e_id) {
        Wave& w = group->sequence.at(e_id).transmit_setup.wave;
        generic_clone_test(w);
      }
    }
  }
}

TEST_CASE("Clone Element", "[Clone]") {
  generic_clone_test(Element());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t p_id = 0; p_id < d->acquisition.groups.size(); ++p_id) {
    auto& probe = d->acquisition.probes.at(p_id);
    if (probe != nullptr) {
      for (size_t e_id = 0; e_id < probe->elements.size(); ++e_id) {
        Element& e = probe->elements.at(e_id);
        generic_clone_test(e);
      }
    }
  }
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

TEST_CASE("Clone TransmitSetup", "[Clone]") {
  TransmitSetup ts;
  generic_clone_test(ts);

  Wave w;
  w.type = urx::WaveType::DIVERGING_WAVE;
  w.time_zero = -42.42;
  w.time_zero_reference_point = {1.23, 4.56, 7.89};
  w.parameters = {1.23, 4.56, 7.89, 10.11, 12.13};

  std::shared_ptr<Excitation> ex = std::make_shared<Excitation>();
  std::shared_ptr<Excitation> ex_2 = std::make_shared<Excitation>();
  ex_2->pulse_shape = "Something";
  ex_2->sampling_frequency = 120e6;
  ex_2->transmit_frequency = 10e6;
  ex_2->waveform = {0, 1.2, -3.4, .56, -7, 89};

  std::shared_ptr<Probe> p = std::make_shared<Probe>();

  ts.wave = w;
  ts.time_offset = 10e3;
  ts.probe_transform = {{1.23, 4.56, 7.89}, {10, -11.1, 12.3}};
  ts.active_elements = {{}, {1, 2}, {1, 4, 789}};
  ts.delays = {1.23, 4.56, 7.89, 10.11, 12.13};
  ts.excitations = {ex, std::weak_ptr<Excitation>(), ex_2};
  ts.probe = p;

  generic_clone_test(ts);
}

}  // namespace urx::utils::test