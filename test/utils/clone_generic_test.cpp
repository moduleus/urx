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
  genericCloneTest(d);
  d = 42.42;
  genericCloneTest(d);
}

TEST_CASE("Clone Vector2D", "[Clone]") {
  Vector2D<double> const d_vec = {1.23, 4.56};
  genericCloneTest(d_vec);
}

TEST_CASE("Clone Vector3D", "[Clone]") {
  Vector3D<double> const d_vec = {1.23, 4.56, -7.89};
  genericCloneTest(d_vec);
}

TEST_CASE("Clone Version", "[Clone]") {
  Version const vers = {1, 2, 3};
  genericCloneTest(vers);
}

TEST_CASE("Clone Transform", "[Clone]") {
  Transform const t = {{1, 2, 3}, {4, 5, 6}};
  genericCloneTest(t);
}

TEST_CASE("Clone Wave", "[Clone]") {
  genericCloneTest(Wave());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (auto& e : group->sequence) {
        Wave const& w = e.transmit_setup.wave;
        genericCloneTest(w);
      }
    }
  }
}

TEST_CASE("Clone Element", "[Clone]") {
  genericCloneTest(Element());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t p_id = 0; p_id < d->acquisition.groups.size(); ++p_id) {
    auto& probe = d->acquisition.probes.at(p_id);
    if (probe != nullptr) {
      for (auto& e : probe->elements) {
        genericCloneTest(e);
      }
    }
  }
}

TEST_CASE("Clone ElementGeometry", "[Clone]") {
  genericCloneTest(ElementGeometry());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t p_id = 0; p_id < d->acquisition.groups.size(); ++p_id) {
    auto& probe = d->acquisition.probes.at(p_id);
    if (probe != nullptr) {
      for (const auto& eg : probe->element_geometries) {
        if (eg) {
          genericCloneTest(*eg);
        }
      }
    }
  }
}

TEST_CASE("Clone ImpulseResponse", "[Clone]") {
  genericCloneTest(ImpulseResponse());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (size_t p_id = 0; p_id < d->acquisition.groups.size(); ++p_id) {
    auto& probe = d->acquisition.probes.at(p_id);
    if (probe != nullptr) {
      for (const auto& ir : probe->impulse_responses) {
        if (ir) {
          genericCloneTest(*ir);
        }
      }
    }
  }
}

TEST_CASE("Clone TransmitSetup", "[Clone]") {
  genericCloneTest(TransmitSetup());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (auto& e : group->sequence) {
        TransmitSetup const& ts = e.transmit_setup;
        genericCloneTest(ts);
      }
    }
  }
}

TEST_CASE("Clone ReceiveSetup", "[Clone]") {
  genericCloneTest(ReceiveSetup());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (auto& e : group->sequence) {
        ReceiveSetup const& rs = e.receive_setup;
        genericCloneTest(rs);
      }
    }
  }
}

TEST_CASE("Clone Event", "[Clone]") {
  genericCloneTest(Event());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (auto& e : group->sequence) {
        genericCloneTest(e);
      }
    }
  }
}

TEST_CASE("Clone Excitation", "[Clone]") {
  genericCloneTest(Excitation());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (auto& ex : d->acquisition.excitations) {
    if (ex != nullptr) {
      genericCloneTest(*ex);
    }
  }
}

TEST_CASE("Clone Group", "[Clone]") {
  genericCloneTest(Group());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (auto& g : d->acquisition.groups) {
    if (g != nullptr) {
      genericCloneTest(*g);
    }
  }
}

TEST_CASE("Clone GroupData", "[Clone]") {
  genericCloneTest(GroupData());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (auto& gd : d->acquisition.groups_data) {
    genericCloneTest(gd);
  }
}

TEST_CASE("Clone RawData", "[Clone]") {
  auto d = utils::test::generateWrongDataset<Dataset>();

  std::vector<std::shared_ptr<RawData>> rd_vec;
  rd_vec.reserve(d->acquisition.groups_data.size());
  for (auto& gd_id : d->acquisition.groups_data) {
    rd_vec.push_back(gd_id.raw_data);
  }
  // NULLPTR
  rd_vec.push_back(nullptr);

  // Random gen
  std::random_device rd;
  std::mt19937 gen(
      rd());  // these can be global and/or static, depending on how you use random elsewhere

  // RawDataVector with no data
  std::vector<std::complex<float>> const no_data_vec(0);
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

  for (auto& rd : rd_vec) {
    if (rd) {
      std::shared_ptr<RawData> rd_cloned = clone<RawData, std::shared_ptr<RawData>>(*rd);
      REQUIRE(rd_cloned == rd);
      if (rd) {
        REQUIRE(rd_cloned->getSize() == rd->getSize());
        REQUIRE(rd_cloned->getSamplingType() == rd->getSamplingType());
        REQUIRE(rd_cloned->getDataType() == rd->getDataType());
        if (dynamic_cast<RawDataWeak<std::complex<double>>*>(rd.get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<std::complex<float>>*>(rd.get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<std::complex<int32_t>>*>(rd.get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<std::complex<int16_t>>*>(rd.get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<double>*>(rd.get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<float>*>(rd.get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<int32_t>*>(rd.get())) {
          REQUIRE(rd_cloned->getBuffer() == rd->getBuffer());
        } else if (dynamic_cast<RawDataWeak<int16_t>*>(rd.get())) {
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
  genericCloneTest(Probe());
  auto d = utils::test::generateWrongDataset<Dataset>();

  for (auto& p : d->acquisition.probes) {
    if (p != nullptr) {
      if (p->description != "Probe with wrong ptr") {
        genericCloneTest(*p);
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
        int32_t const eg_id = getElementIndex(p_cloned->element_geometries,
                                              p_cloned->elements.at(e_id).element_geometry);
        if (eg_id < 0) {
          REQUIRE(p_cloned->elements.at(e_id).element_geometry == std::weak_ptr<ElementGeometry>());
        } else {
          REQUIRE(p_cloned->elements.at(e_id).element_geometry ==
                  p_cloned->element_geometries.at(eg_id));
          REQUIRE(p_cloned->elements.at(e_id).element_geometry == p->element_geometries.at(eg_id));
          REQUIRE(-1 != getElementIndex(p->element_geometries,
                                        p_cloned->elements.at(e_id).element_geometry));
        }

        int32_t const ir_id = getElementIndex(p_cloned->impulse_responses,
                                              p_cloned->elements.at(e_id).impulse_response);
        if (eg_id < 0) {
          REQUIRE(p_cloned->elements.at(e_id).impulse_response == std::weak_ptr<ImpulseResponse>());
        } else {
          REQUIRE(p_cloned->elements.at(e_id).impulse_response ==
                  p_cloned->impulse_responses.at(ir_id));
          REQUIRE(p_cloned->elements.at(e_id).impulse_response == p->impulse_responses.at(ir_id));
          REQUIRE(-1 != getElementIndex(p->impulse_responses,
                                        p_cloned->elements.at(e_id).impulse_response));
        }
      }
    }
  }
}

TEST_CASE("Clone Acquisition", "[Clone]") {
  genericCloneTest(Acquisition());
  auto d = utils::test::generateFakeDataset<Dataset>();
  auto acq = d->acquisition;
  auto acq_cloned = utils::clone(acq);
  REQUIRE(acq == acq_cloned);
  REQUIRE(std::is_same_v<decltype(acq), decltype(acq_cloned)>);

  d = utils::test::generateWrongDataset<Dataset>();
  acq = d->acquisition;
  acq_cloned = utils::clone(acq);
  REQUIRE(acq != acq_cloned);
  REQUIRE(std::is_same_v<decltype(acq), decltype(acq_cloned)>);

  for (size_t p_id = 0; p_id < acq.probes.size(); ++p_id) {
    if (acq.probes.at(p_id)->description == "Probe with wrong ptr") {
      acq.probes.erase(acq.probes.begin() + p_id);
      break;
    }
  }
  genericCloneTest(acq);
  acq_cloned = utils::clone(acq);

  for (size_t p_id = 0; p_id < acq.probes.size(); ++p_id) {
    REQUIRE(acq_cloned.probes.at(p_id) == acq.probes.at(p_id));
    if (acq.probes.at(p_id).get()) {
      REQUIRE(acq_cloned.probes.at(p_id).get() != acq.probes.at(p_id).get());
    } else {
      REQUIRE(acq_cloned.probes.at(p_id).get() == acq.probes.at(p_id).get());
    }
  }
  for (size_t ex_id = 0; ex_id < acq.excitations.size(); ++ex_id) {
    REQUIRE(acq_cloned.excitations.at(ex_id) == acq.excitations.at(ex_id));
    if (acq.excitations.at(ex_id).get()) {
      REQUIRE(acq_cloned.excitations.at(ex_id).get() != acq.excitations.at(ex_id).get());
    } else {
      REQUIRE(acq_cloned.excitations.at(ex_id).get() == acq.excitations.at(ex_id).get());
    }
  }
  for (size_t g_id = 0; g_id < acq.groups.size(); ++g_id) {
    REQUIRE(acq_cloned.groups.at(g_id) == acq.groups.at(g_id));
    if (acq.groups.at(g_id).get()) {
      REQUIRE(acq_cloned.groups.at(g_id).get() != acq.groups.at(g_id).get());
    } else {
      REQUIRE(acq_cloned.groups.at(g_id).get() == acq.groups.at(g_id).get());
    }
  }
  for (size_t gd_id = 0; gd_id < acq.groups_data.size(); ++gd_id) {
    REQUIRE(acq_cloned.groups_data.at(gd_id) == acq.groups_data.at(gd_id));
    if (acq.groups_data.at(gd_id).raw_data.get()) {
      REQUIRE(acq_cloned.groups_data.at(gd_id).raw_data.get() !=
              acq.groups_data.at(gd_id).raw_data.get());
    } else {
      REQUIRE(acq_cloned.groups_data.at(gd_id).raw_data.get() ==
              acq.groups_data.at(gd_id).raw_data.get());
    }

    int32_t const g_id = getElementIndex(acq.groups, acq.groups_data.at(gd_id).group);
    int32_t const cloned_g_id =
        getElementIndex(acq_cloned.groups, acq_cloned.groups_data.at(gd_id).group);
    REQUIRE(acq_cloned.groups_data.at(gd_id) == acq.groups_data.at(gd_id));
    REQUIRE(g_id == cloned_g_id);
    if (g_id < 0) {
      REQUIRE(acq_cloned.groups_data.at(gd_id).group == std::weak_ptr<Group>());
    } else {
      REQUIRE(acq_cloned.groups_data.at(gd_id).group.lock().get() ==
              acq_cloned.groups.at(cloned_g_id).get());
      REQUIRE(acq.groups_data.at(gd_id).group.lock().get() == acq.groups.at(cloned_g_id).get());
      REQUIRE(acq_cloned.groups_data.at(gd_id).group.lock().get() !=
              acq.groups_data.at(gd_id).group.lock().get());
    }
  }

  for (size_t g_id = 0; g_id < acq.groups.size(); ++g_id) {
    if (acq_cloned.groups.at(g_id)) {
      auto& group = acq_cloned.groups.at(g_id);
      for (auto& e : group->sequence) {
        auto& receive_setup = e.receive_setup;
        auto& transmit_setup = e.transmit_setup;

        int32_t p_id = getElementIndex(acq.probes, receive_setup.probe);
        if (p_id < 0) {
          receive_setup.probe = std::weak_ptr<Probe>();
        } else {
          receive_setup.probe = acq_cloned.probes.at(p_id);
        }

        p_id = getElementIndex(acq.probes, transmit_setup.probe);
        if (p_id < 0) {
          transmit_setup.probe = std::weak_ptr<Probe>();
        } else {
          transmit_setup.probe = acq_cloned.probes.at(p_id);
        }
        for (auto& excitation : transmit_setup.excitations) {
          int32_t const acq_ex_id = getElementIndex(acq.excitations, excitation);
          if (acq_ex_id < 0) {
            excitation = std::weak_ptr<Excitation>();
          } else {
            excitation = acq_cloned.excitations.at(acq_ex_id);
          }
        }
      }
    }
  }
}

TEST_CASE("Clone Dataset", "[Clone]") {
  genericCloneTest(Dataset());
  auto d = utils::test::generateFakeDataset<Dataset>();
  genericCloneTest(*d);
  genericCloneTest(d);
}

}  // namespace urx::utils::test