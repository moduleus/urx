#include "clone_generic_test.h"

#include <algorithm>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
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
#include <urx/utils/common.h>
#include <urx/utils/detail/clone.h>
#include <urx/utils/test/dataset_gen.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx::utils::test {

TEST_CASE("Clone DoubleNan", "[Clone]") {
  DoubleNan d;
  genericCloneTest(d, false);
  d = 42.42;
  genericCloneTest(d, false);
}

TEST_CASE("Clone Vector2D", "[Clone]") {
  const Vector2D<double> d_vec = {1.23, 4.56};
  genericCloneTest(d_vec, false);
}

TEST_CASE("Clone Vector3D", "[Clone]") {
  const Vector3D<double> d_vec = {1.23, 4.56, -7.89};
  genericCloneTest(d_vec, false);
}

TEST_CASE("Clone Version", "[Clone]") {
  const Version vers = {1, 2, 3};
  genericCloneTest(vers, false);
}

TEST_CASE("Clone Transform", "[Clone]") {
  const Transform t = {{1, 2, 3}, {4, 5, 6}};
  genericCloneTest(t, false);
}

TEST_CASE("Clone Wave", "[Clone]") {
  genericCloneTest(Wave(), true);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (auto& e : group->sequence) {
        const Wave& w = e.transmit_setup.wave;
        genericCloneTest(w, true);
      }
    }
  }
}

TEST_CASE("Clone Element", "[Clone]") {
  genericCloneTest(Element(), false);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& probe : d->acquisition.probes) {
    if (probe != nullptr) {
      for (const auto& e : probe->elements) {
        genericCloneTest(e, false);
      }
    }
  }
}

TEST_CASE("Clone ElementGeometry", "[Clone]") {
  genericCloneTest(ElementGeometry(), true);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& probe : d->acquisition.probes) {
    if (probe != nullptr) {
      for (const auto& eg : probe->element_geometries) {
        if (eg) {
          genericCloneTest(*eg, true);
        }
      }
    }
  }
}

TEST_CASE("Clone ImpulseResponse", "[Clone]") {
  genericCloneTest(ImpulseResponse(), true);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& probe : d->acquisition.probes) {
    if (probe != nullptr) {
      for (const auto& ir : probe->impulse_responses) {
        if (ir) {
          genericCloneTest(*ir, true);
        }
      }
    }
  }
}

TEST_CASE("Clone TransmitSetup", "[Clone]") {
  genericCloneTest(TransmitSetup(), false);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (auto& e : group->sequence) {
        const TransmitSetup& ts = e.transmit_setup;
        genericCloneTest(ts, false);
      }
    }
  }
}

TEST_CASE("Clone ReceiveSetup", "[Clone]") {
  genericCloneTest(ReceiveSetup(), false);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (const auto& e : group->sequence) {
        const ReceiveSetup& rs = e.receive_setup;
        genericCloneTest(rs, false);
      }
    }
  }
}

TEST_CASE("Clone Event", "[Clone]") {
  genericCloneTest(Event(), false);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& group : d->acquisition.groups) {
    if (group != nullptr) {
      for (const auto& e : group->sequence) {
        genericCloneTest(e, false);
      }
    }
  }
}

TEST_CASE("Clone Excitation", "[Clone]") {
  genericCloneTest(Excitation(), true);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& ex : d->acquisition.excitations) {
    if (ex != nullptr) {
      genericCloneTest(*ex, true);
    }
  }
}

TEST_CASE("Clone Group", "[Clone]") {
  genericCloneTest(Group(), true);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& g : d->acquisition.groups) {
    if (g != nullptr) {
      genericCloneTest(*g, true);
    }
  }
}

TEST_CASE("Clone GroupData", "[Clone]") {
  genericCloneTest(GroupData(), true);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& gd : d->acquisition.groups_data) {
    genericCloneTest(gd, true);
  }
}

TEST_CASE("Clone RawData", "[Clone]") {
  const auto d = utils::test::generateWrongDataset<Dataset>();

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
  std::vector<std::complex<float>> no_data_vec(0);
  rd_vec.push_back(std::make_shared<RawDataVector<std::complex<float>>>(std::move(no_data_vec)));

  // RawDataVector with 1000 std::complex<float>
  std::vector<std::complex<float>> com_float_data_vec(1000);
  std::uniform_real_distribution<> real_dis(-1e9, 1e9);
  std::generate(com_float_data_vec.begin(), com_float_data_vec.end(),
                [&]() { return std::complex<float>(real_dis(gen), real_dis(gen)); });
  const auto rd_vec_comp_float =
      std::make_shared<RawDataVector<std::complex<float>>>(std::move(com_float_data_vec));
  rd_vec.push_back(rd_vec_comp_float);

  // RawDataVector with 1000 int16_t
  std::vector<int16_t> data_vec(1000);
  std::uniform_int_distribution<> int_dis(-1e9, 1e9);
  std::generate(data_vec.begin(), data_vec.end(), [&]() { return int_dis(gen); });
  const auto rd_vec_int16 = std::make_shared<RawDataVector<int16_t>>(std::move(data_vec));
  rd_vec.push_back(rd_vec_int16);

  // RawDataNoInit with no data
  rd_vec.push_back(std::make_shared<RawDataNoInit<int32_t>>(0));
  rd_vec.push_back(std::make_shared<RawDataNoInit<std::complex<int16_t>>>(0));

  // RawDataNoInit with 1000 int16_t not initialized
  rd_vec.push_back(std::make_shared<RawDataNoInit<int16_t>>(1000));

  // RawDataNoInit with 1000 std::complex<double> not initialized
  const auto rd_no_init_comp_double = std::make_shared<RawDataNoInit<std::complex<double>>>(1000);
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

  for (const auto& rd : rd_vec) {
    if (rd) {
      const std::shared_ptr<RawData> rd_cloned =
          utils::detail::clone<RawData, std::shared_ptr<RawData>>(*rd);
      REQUIRE(valueComparison(rd_cloned, rd));
      REQUIRE(*rd_cloned == *rd);
      REQUIRE(rd_cloned != rd);
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
  genericCloneTest(Probe(), true);
  const auto d = utils::test::generateWrongDataset<Dataset>();

  for (const auto& p : d->acquisition.probes) {
    if (p != nullptr) {
      if (p->description != "Probe with wrong ptr") {
        genericCloneTest(*p, true);
      }
      const auto p_cloned = utils::detail::clone(p);

      REQUIRE(p_cloned->element_geometries.size() == p->element_geometries.size());
      REQUIRE(p_cloned->impulse_responses.size() == p->impulse_responses.size());
      REQUIRE(p_cloned->elements.size() == p->elements.size());

      for (size_t eg_id = 0; eg_id < p->element_geometries.size(); ++eg_id) {
        REQUIRE(
            valueComparison(p_cloned->element_geometries[eg_id], p->element_geometries.at(eg_id)));
        if (p->element_geometries.at(eg_id)) {
          REQUIRE(*p_cloned->element_geometries[eg_id] == *p->element_geometries.at(eg_id));
          REQUIRE(p_cloned->element_geometries[eg_id].get() !=
                  p->element_geometries.at(eg_id).get());
        }
      }
      for (size_t ir_id = 0; ir_id < p->impulse_responses.size(); ++ir_id) {
        REQUIRE(
            valueComparison(p_cloned->impulse_responses[ir_id], p->impulse_responses.at(ir_id)));
        if (p->impulse_responses.at(ir_id)) {
          REQUIRE(*p_cloned->impulse_responses[ir_id] == *p->impulse_responses.at(ir_id));
          REQUIRE(p_cloned->impulse_responses[ir_id].get() != p->impulse_responses.at(ir_id).get());
        }
      }
      for (size_t e_id = 0; e_id < p_cloned->elements.size(); ++e_id) {
        std::optional<size_t> const eg_id = common::getElementIndex(
            p_cloned->element_geometries, p_cloned->elements.at(e_id).element_geometry, false);
        if (!eg_id.has_value()) {
          REQUIRE(p_cloned->elements.at(e_id).element_geometry.expired());
        } else {
          REQUIRE(valueComparison(p_cloned->elements.at(e_id).element_geometry,
                                  p_cloned->element_geometries.at(eg_id.value())));
          REQUIRE(valueComparison(p_cloned->elements.at(e_id).element_geometry,
                                  p->element_geometries.at(eg_id.value())));

          REQUIRE(std::nullopt !=
                  common::getElementIndex(p->element_geometries,
                                          p_cloned->elements.at(e_id).element_geometry, true));
          REQUIRE(std::nullopt != common::getElementIndex(
                                      p->element_geometries,
                                      p_cloned->elements.at(e_id).element_geometry.lock(), true));
          REQUIRE(std::nullopt ==
                  common::getElementIndex(p->element_geometries,
                                          p_cloned->elements.at(e_id).element_geometry, false));
          REQUIRE(std::nullopt == common::getElementIndex(
                                      p->element_geometries,
                                      p_cloned->elements.at(e_id).element_geometry.lock(), false));
        }

        std::optional<size_t> const ir_id = common::getElementIndex(
            p_cloned->impulse_responses, p_cloned->elements.at(e_id).impulse_response, true);
        if (!ir_id.has_value()) {
          REQUIRE(p_cloned->elements.at(e_id).impulse_response.expired());
        } else {
          REQUIRE(valueComparison(p_cloned->elements.at(e_id).impulse_response,
                                  p_cloned->impulse_responses.at(ir_id.value())));
          REQUIRE(valueComparison(p_cloned->elements.at(e_id).impulse_response,
                                  p->impulse_responses.at(ir_id.value())));
          REQUIRE(std::nullopt !=
                  common::getElementIndex(p->impulse_responses,
                                          p_cloned->elements.at(e_id).impulse_response, true));
          REQUIRE(std::nullopt != common::getElementIndex(
                                      p->impulse_responses,
                                      p_cloned->elements.at(e_id).impulse_response.lock(), true));
          REQUIRE(std::nullopt ==
                  common::getElementIndex(p->impulse_responses,
                                          p_cloned->elements.at(e_id).impulse_response, false));
          REQUIRE(std::nullopt == common::getElementIndex(
                                      p->impulse_responses,
                                      p_cloned->elements.at(e_id).impulse_response.lock(), false));
        }
      }
    }
  }
}

void testAcquisitionClone(const Acquisition& acq, const Acquisition& acq_cloned) {
  // Check Acquisition::probes
  REQUIRE(valueComparison(acq_cloned.probes, acq.probes));
  for (size_t p_id = 0; p_id < acq.probes.size(); ++p_id) {
    REQUIRE(valueComparison(acq_cloned.probes.at(p_id), acq.probes.at(p_id)));
    if (acq.probes.at(p_id).get()) {
      REQUIRE(acq_cloned.probes.at(p_id).get() != acq.probes.at(p_id).get());
    } else {
      REQUIRE(acq_cloned.probes.at(p_id).get() == acq.probes.at(p_id).get());
    }
  }

  // Check Acquisition::excitations
  REQUIRE(valueComparison(acq_cloned.excitations, acq.excitations));
  for (size_t ex_id = 0; ex_id < acq.excitations.size(); ++ex_id) {
    REQUIRE(valueComparison(acq_cloned.excitations.at(ex_id), acq.excitations.at(ex_id)));
    if (acq.excitations.at(ex_id).get()) {
      REQUIRE(acq_cloned.excitations.at(ex_id).get() != acq.excitations.at(ex_id).get());
    } else {
      REQUIRE(acq_cloned.excitations.at(ex_id).get() == acq.excitations.at(ex_id).get());
    }
  }

  // Check Acquisition::groups
  REQUIRE(valueComparison(acq_cloned.groups, acq.groups));
  for (size_t g_id = 0; g_id < acq.groups.size(); ++g_id) {
    REQUIRE(valueComparison(acq_cloned.groups.at(g_id), acq.groups.at(g_id)));
    if (acq.groups.at(g_id).get()) {
      REQUIRE(acq_cloned.groups.at(g_id).get() != acq.groups.at(g_id).get());
    } else {
      REQUIRE(acq_cloned.groups.at(g_id).get() == acq.groups.at(g_id).get());
    }
  }

  // Check Acquisition::groups_data
  REQUIRE(valueComparison(acq_cloned.groups_data, acq.groups_data));
  for (size_t gd_id = 0; gd_id < acq.groups_data.size(); ++gd_id) {
    REQUIRE(valueComparison(acq_cloned.groups_data.at(gd_id).raw_data,
                            acq.groups_data.at(gd_id).raw_data));
    if (acq.groups_data.at(gd_id).raw_data.get()) {
      REQUIRE(acq_cloned.groups_data.at(gd_id).raw_data.get() !=
              acq.groups_data.at(gd_id).raw_data.get());
    } else {
      REQUIRE(acq_cloned.groups_data.at(gd_id).raw_data.get() ==
              acq.groups_data.at(gd_id).raw_data.get());
    }

    // Check GroupData::group
    std::optional<size_t> const g_id =
        common::getElementIndex(acq.groups, acq.groups_data.at(gd_id).group, false);
    std::optional<size_t> const cloned_g_id =
        common::getElementIndex(acq_cloned.groups, acq_cloned.groups_data.at(gd_id).group, false);
    REQUIRE(acq_cloned.groups_data.at(gd_id) == acq.groups_data.at(gd_id));
    REQUIRE(g_id == cloned_g_id);
    if (!g_id.has_value()) {
      REQUIRE(acq_cloned.groups_data.at(gd_id).group.expired());
    } else {
      REQUIRE(acq_cloned.groups_data.at(gd_id).group.lock().get() ==
              acq_cloned.groups.at(*g_id).get());
      REQUIRE(acq.groups_data.at(gd_id).group.lock().get() == acq.groups.at(*g_id).get());
      REQUIRE(acq_cloned.groups_data.at(gd_id).group.lock().get() !=
              acq.groups_data.at(gd_id).group.lock().get());
    }
  }

  for (size_t g_id = 0; g_id < acq.groups.size(); ++g_id) {
    const auto& group = acq.groups.at(g_id);
    const auto& group_cloned = acq_cloned.groups.at(g_id);
    if (group) {
      REQUIRE(group->sequence.size() == group_cloned->sequence.size());
      for (size_t evt_id = 0; evt_id < group_cloned->sequence.size(); ++evt_id) {
        const auto& evt = group->sequence.at(evt_id);
        const auto& evt_cloned = group_cloned->sequence.at(evt_id);

        const auto& receive_setup_cloned = evt_cloned.receive_setup;
        const auto& transmit_setup_cloned = evt_cloned.transmit_setup;

        const auto& receive_setup = evt.receive_setup;
        const auto& transmit_setup = evt.transmit_setup;

        // Check ReceiveSetup::probe
        std::optional<size_t> p_id =
            common::getElementIndex(acq.probes, receive_setup.probe, false);
        std::optional<size_t> p_id_cloned =
            common::getElementIndex(acq_cloned.probes, receive_setup_cloned.probe, false);
        REQUIRE(p_id == p_id_cloned);
        if (!p_id.has_value()) {
          REQUIRE(receive_setup_cloned.probe.expired());
        } else {
          REQUIRE(valueComparison(receive_setup_cloned.probe, acq_cloned.probes.at(*p_id)));
          REQUIRE(valueComparison(receive_setup_cloned.probe, acq.probes.at(*p_id)));
        }

        // Check TansmitSetup::probe
        p_id = common::getElementIndex(acq.probes, transmit_setup.probe, false);
        p_id_cloned =
            common::getElementIndex(acq_cloned.probes, transmit_setup_cloned.probe, false);
        REQUIRE(p_id == p_id_cloned);
        if (!p_id.has_value()) {
          REQUIRE(transmit_setup_cloned.probe.expired());
        } else {
          REQUIRE(valueComparison(transmit_setup_cloned.probe, acq_cloned.probes.at(*p_id)));
          REQUIRE(valueComparison(transmit_setup_cloned.probe, acq.probes.at(*p_id)));
        }

        // Check TansmitSetup::excitation
        REQUIRE(transmit_setup.excitations.size() == transmit_setup_cloned.excitations.size());
        for (size_t ex_id = 0; ex_id < transmit_setup.excitations.size(); ++ex_id) {
          const auto& excitation = transmit_setup.excitations.at(ex_id);
          const auto& excitation_cloned = transmit_setup_cloned.excitations.at(ex_id);
          const std::optional<size_t> acq_ex_id =
              common::getElementIndex(acq.excitations, excitation, false);
          const std::optional<size_t> acq_cloned_ex_id =
              common::getElementIndex(acq_cloned.excitations, excitation_cloned, false);

          REQUIRE(acq_ex_id == acq_cloned_ex_id);
          if (!acq_ex_id.has_value()) {
            REQUIRE(excitation_cloned.expired());
          } else {
            REQUIRE(valueComparison(excitation_cloned, acq_cloned.excitations.at(*acq_ex_id)));
            REQUIRE(valueComparison(excitation_cloned, acq.excitations.at(*acq_ex_id)));
          }
        }
      }
    }
  }
}

TEST_CASE("Clone Acquisition", "[Clone]") {
  genericCloneTest(Acquisition(), false);
  auto d = utils::test::generateFakeDataset<Dataset>();
  auto acq = d->acquisition;
  auto acq_cloned = utils::detail::clone(acq);
  REQUIRE(acq == acq_cloned);
  REQUIRE(std::is_same_v<decltype(acq), decltype(acq_cloned)>);

  testAcquisitionClone(acq, acq_cloned);

  d = utils::test::generateWrongDataset<Dataset>();
  acq = d->acquisition;
  acq_cloned = utils::detail::clone(acq);
  // Wrong Dataset contains pointers in a probe that does not belong to it causing the non equal comparison
  REQUIRE(acq != acq_cloned);
  REQUIRE(std::is_same_v<decltype(acq), decltype(acq_cloned)>);

  for (size_t p_id = 0; p_id < acq.probes.size(); ++p_id) {
    if (acq.probes.at(p_id)->description == "Probe with wrong ptr") {
      acq.probes.erase(acq.probes.begin() + p_id);
      break;
    }
  }
  genericCloneTest(acq, false);
  acq_cloned = utils::detail::clone(acq);
  testAcquisitionClone(acq, acq_cloned);
}

TEST_CASE("Clone Dataset", "[Clone]") {
  genericCloneTest(Dataset(), true);
  const std::shared_ptr<Dataset> d = utils::test::generateFakeDataset<Dataset>();
  genericCloneTest(*d, true);

  const std::shared_ptr<Dataset> d_cloned = utils::detail::clone(d);
  testAcquisitionClone(d->acquisition, d_cloned->acquisition);
}

}  // namespace urx::utils::test