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
  generic_clone_test(ElementGeometry());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t p_id = 0; p_id < d->acquisition.groups.size(); ++p_id) {
    auto& probe = d->acquisition.probes.at(p_id);
    if (probe != nullptr) {
      for (size_t eg_id = 0; eg_id < probe->element_geometries.size(); ++eg_id) {
        std::shared_ptr<ElementGeometry> eg = probe->element_geometries.at(eg_id);
        if (eg) {
          generic_clone_test(*eg);
        }
      }
    }
  }
}

TEST_CASE("Clone ImpulseResponse", "[Clone]") {
  generic_clone_test(ImpulseResponse());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t p_id = 0; p_id < d->acquisition.groups.size(); ++p_id) {
    auto& probe = d->acquisition.probes.at(p_id);
    if (probe != nullptr) {
      for (size_t ir_id = 0; ir_id < probe->impulse_responses.size(); ++ir_id) {
        std::shared_ptr<ImpulseResponse> ir = probe->impulse_responses.at(ir_id);
        if (ir) {
          generic_clone_test(*ir);
        }
      }
    }
  }
}

TEST_CASE("Clone TransmitSetup", "[Clone]") {
  generic_clone_test(TransmitSetup());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t g_id = 0; g_id < d->acquisition.groups.size(); ++g_id) {
    auto& group = d->acquisition.groups.at(g_id);
    if (group != nullptr) {
      for (size_t e_id = 0; e_id < group->sequence.size(); ++e_id) {
        TransmitSetup& ts = group->sequence.at(e_id).transmit_setup;
        generic_clone_test(ts);
      }
    }
  }
}

TEST_CASE("Clone ReceiveSetup", "[Clone]") {
  generic_clone_test(ReceiveSetup());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t g_id = 0; g_id < d->acquisition.groups.size(); ++g_id) {
    auto& group = d->acquisition.groups.at(g_id);
    if (group != nullptr) {
      for (size_t e_id = 0; e_id < group->sequence.size(); ++e_id) {
        ReceiveSetup& rs = group->sequence.at(e_id).receive_setup;
        generic_clone_test(rs);
      }
    }
  }
}

TEST_CASE("Clone Event", "[Clone]") {
  generic_clone_test(Event());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t g_id = 0; g_id < d->acquisition.groups.size(); ++g_id) {
    auto& group = d->acquisition.groups.at(g_id);
    if (group != nullptr) {
      for (size_t e_id = 0; e_id < group->sequence.size(); ++e_id) {
        Event& e = group->sequence.at(e_id);
        generic_clone_test(e);
      }
    }
  }
}

}  // namespace urx::utils::test