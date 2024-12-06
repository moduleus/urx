#include <algorithm>
#include <memory>
#include <random>

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

TEST_CASE("Clone Excitation", "[Clone]") {
  generic_clone_test(Excitation());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t ex_id = 0; ex_id < d->acquisition.excitations.size(); ++ex_id) {
    auto& ex = d->acquisition.excitations.at(ex_id);
    if (ex != nullptr) {
      generic_clone_test(*ex);
    }
  }
}

TEST_CASE("Clone Group", "[Clone]") {
  generic_clone_test(Group());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t g_id = 0; g_id < d->acquisition.groups.size(); ++g_id) {
    auto& g = d->acquisition.groups.at(g_id);
    if (g != nullptr) {
      generic_clone_test(*g);
    }
  }
}

TEST_CASE("Clone GroupData", "[Clone]") {
  generic_clone_test(GroupData());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t gd_id = 0; gd_id < d->acquisition.groups_data.size(); ++gd_id) {
    auto& gd = d->acquisition.groups_data.at(gd_id);
    generic_clone_test(gd);
  }
}

TEST_CASE("Clone RawData", "[Clone]") {
  auto d = utils::test::generateWrongDataset<Dataset>();

  std::vector<std::shared_ptr<RawData>> rd_vec;
  rd_vec.reserve(d->acquisition.groups_data.size());
  for (size_t gd_id = 0; gd_id < d->acquisition.groups_data.size(); ++gd_id) {
    rd_vec.push_back(d->acquisition.groups_data.at(gd_id).raw_data);
  }
  // NULLPTR
  rd_vec.push_back(nullptr);

  // Random gen
  std::random_device rd;
  std::mt19937 gen(
      rd());  // these can be global and/or static, depending on how you use random elsewhere

  // RawDataVector with no data
  std::vector<std::complex<float>> no_data_vec(0);
  rd_vec.push_back(std::make_shared<RawDataVector<std::complex<float>>>(no_data_vec));

  // RawDataVector with 1000 std::complex<float>
  std::vector<std::complex<float>> com_float_data_vec(1000);
  std::uniform_real_distribution<> real_dis(-1e9, 1e9);
  std::generate(com_float_data_vec.begin(), com_float_data_vec.end(),
                [&]() { return std::complex<float>(real_dis(gen), real_dis(gen)); });
  auto rd_vec_comp_float = std::make_shared<RawDataVector<std::complex<float>>>(com_float_data_vec);
  rd_vec.push_back(rd_vec_comp_float);

  // RawDataVector with 1000 int16_t
  std::vector<int16_t> data_vec(1000);
  std::uniform_int_distribution<> int_dis(-1e9, 1e9);
  std::generate(data_vec.begin(), data_vec.end(), [&]() { return int_dis(gen); });
  auto rd_vec_int16 = std::make_shared<RawDataVector<int16_t>>(data_vec);
  rd_vec.push_back(rd_vec_int16);

  // RawDataNoInit with no data
  rd_vec.push_back(std::make_shared<RawDataNoInit<int32_t>>(0));
  rd_vec.push_back(std::make_shared<RawDataNoInit<std::complex<int16_t>>>(0));

  // RawDataNoInit with 1000 int16_t not initialized
  rd_vec.push_back(std::make_shared<RawDataNoInit<int16_t>>(1000));

  // RawDataNoInit with 1000 std::complex<double> not initialized
  auto rd_no_init_comp_double = std::make_shared<RawDataNoInit<std::complex<double>>>(1000);
  rd_vec.push_back(rd_no_init_comp_double);

  // RawDataNoInit with 1000 std::complex<double>
  auto rd_no_init_comp_double_init = std::make_shared<RawDataNoInit<std::complex<double>>>(1000);
  rd_vec.push_back(rd_no_init_comp_double_init);
  std::generate(static_cast<std::complex<double>*>(rd_no_init_comp_double_init->getBuffer()),
                static_cast<std::complex<double>*>(rd_no_init_comp_double_init->getBuffer()) +
                    rd_no_init_comp_double_init->getSize(),
                [&]() { return std::complex<double>(real_dis(gen), real_dis(gen)); });

  // RawDataWeak with 1000 int16
  rd_vec.push_back(
      std::make_shared<RawDataWeak<int16_t>>(rd_vec_int16->getBuffer(), rd_vec_int16->getSize()));

  // RawDataWeak with 1000 int16
  rd_vec.push_back(std::make_shared<RawDataWeak<std::complex<float>>>(
      rd_vec_comp_float->getBuffer(), rd_vec_comp_float->getSize()));

  // RawDataWeak with 1000 int16
  rd_vec.push_back(std::make_shared<RawDataWeak<int16_t>>(rd_vec_comp_float->getBuffer(),
                                                          rd_vec_comp_float->getSize()));

  // RawDataWeak with 1000 complex double no init
  rd_vec.push_back(std::make_shared<RawDataWeak<int16_t>>(rd_no_init_comp_double->getBuffer(),
                                                          rd_no_init_comp_double->getSize()));

  // RawDataWeak with 1000 complex double init
  rd_vec.push_back(std::make_shared<RawDataWeak<int16_t>>(rd_no_init_comp_double_init->getBuffer(),
                                                          rd_no_init_comp_double_init->getSize()));

  for (size_t rd_id = 0; rd_id < rd_vec.size(); ++rd_id) {
    std::shared_ptr<RawData> rd = rd_vec.at(rd_id);
    if (rd) {
      std::shared_ptr<RawData> rd_cloned = std::shared_ptr<RawData>(clone<RawData, RawData*>(*rd));
      REQUIRE(rd_cloned == rd);
      if (rd) {
        REQUIRE(rd_cloned->getSize() == rd->getSize());
        REQUIRE(rd_cloned->getSamplingType() == rd->getSamplingType());
        REQUIRE(rd_cloned->getDataType() == rd->getDataType());
        if (dynamic_cast<RawDataWeak<std::complex<double>>*>(rd_vec.at(rd_id).get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<std::complex<float>>*>(rd_vec.at(rd_id).get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<std::complex<int32_t>>*>(rd_vec.at(rd_id).get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<std::complex<int16_t>>*>(rd_vec.at(rd_id).get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<double>*>(rd_vec.at(rd_id).get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<float>*>(rd_vec.at(rd_id).get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<int32_t>*>(rd_vec.at(rd_id).get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<int16_t>*>(rd_vec.at(rd_id).get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else {
          if (rd_cloned->getBuffer()) {
            REQUIRE(rd_cloned->getBuffer() != rd->getBuffer());
          }
        }
      }
    }
  }
}

TEST_CASE("Clone Probe", "[Clone]") {
  generic_clone_test(Probe());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t p_id = 0; p_id < d->acquisition.probes.size(); ++p_id) {
    auto& p = d->acquisition.probes.at(p_id);
    if (p != nullptr) {
      if (p->description != "Probe with wrong ptr") {
        generic_clone_test(*p);
      }
      auto p_cloned = utils::clone(p);

      REQUIRE(p_cloned->element_geometries.size() == p->element_geometries.size());
      REQUIRE(p_cloned->impulse_responses.size() == p->impulse_responses.size());
      REQUIRE(p_cloned->elements.size() == p->elements.size());

      for (size_t eg_id = 0; eg_id < p->element_geometries.size(); ++eg_id) {
        // Compare shared_ptr<ElementGeometry> does not compare ptr
        REQUIRE(p_cloned->element_geometries[eg_id] == p->element_geometries.at(eg_id));
        if (p->element_geometries.at(eg_id)) {
          REQUIRE(*p_cloned->element_geometries[eg_id] == *p->element_geometries.at(eg_id));
          // Compare shared_ptr<ElementGeometry> ptr
          REQUIRE(p_cloned->element_geometries[eg_id].get() !=
                  p->element_geometries.at(eg_id).get());
        }
      }
      for (size_t ir_id = 0; ir_id < p->impulse_responses.size(); ++ir_id) {
        // Compare shared_ptr<ImpulseResponse> does not compare ptr
        REQUIRE(p_cloned->impulse_responses[ir_id] == p->impulse_responses.at(ir_id));
        if (p->impulse_responses.at(ir_id)) {
          REQUIRE(*p_cloned->impulse_responses[ir_id] == *p->impulse_responses.at(ir_id));
          // Compare shared_ptr<ImpulseResponse> ptr
          REQUIRE(p_cloned->impulse_responses[ir_id].get() != p->impulse_responses.at(ir_id).get());
        }
      }
      for (size_t e_id = 0; e_id < p_cloned->elements.size(); ++e_id) {
        int32_t eg_id =
            getEltId(p_cloned->element_geometries, p_cloned->elements.at(e_id).element_geometry);
        if (eg_id < 0) {
          REQUIRE(p_cloned->elements.at(e_id).element_geometry == std::weak_ptr<ElementGeometry>());
        } else {
          REQUIRE(p_cloned->elements.at(e_id).element_geometry ==
                  p_cloned->element_geometries.at(eg_id));
          REQUIRE(p_cloned->elements.at(e_id).element_geometry == p->element_geometries.at(eg_id));
          REQUIRE(-1 !=
                  getEltId(p->element_geometries, p_cloned->elements.at(e_id).element_geometry));
        }

        int32_t ir_id =
            getEltId(p_cloned->impulse_responses, p_cloned->elements.at(e_id).impulse_response);
        if (eg_id < 0) {
          REQUIRE(p_cloned->elements.at(e_id).impulse_response == std::weak_ptr<ImpulseResponse>());
        } else {
          REQUIRE(p_cloned->elements.at(e_id).impulse_response ==
                  p_cloned->impulse_responses.at(ir_id));
          REQUIRE(p_cloned->elements.at(e_id).impulse_response == p->impulse_responses.at(ir_id));
          REQUIRE(-1 !=
                  getEltId(p->impulse_responses, p_cloned->elements.at(e_id).impulse_response));
        }
      }
    }
  }
}

}  // namespace urx::utils::test