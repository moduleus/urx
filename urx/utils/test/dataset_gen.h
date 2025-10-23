#pragma once

#include <algorithm>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/urx.h>
#include <urx/vector.h>

namespace urx::utils::test {

template <typename T>
std::shared_ptr<T> generateFakeDataset() {
  using ExcitationT = typename decltype(T::acquisition.excitations)::value_type::element_type;
  using GroupT = typename decltype(T::acquisition.groups)::value_type::element_type;
  using EventT = typename decltype(GroupT::sequence)::value_type;

  auto dataset = std::make_shared<T>();

  dataset->version.major = urx::URX_VERSION_MAJOR;
  dataset->version.minor = urx::URX_VERSION_MINOR;
  dataset->version.patch = urx::URX_VERSION_PATCH;

  dataset->acquisition.authors = "AuthOr";
  dataset->acquisition.description = "康Description🐜";
  dataset->acquisition.local_time = "2019-06-15T15:53:00";
  dataset->acquisition.country_code = "FR";
  dataset->acquisition.system = "???";
  dataset->acquisition.timestamp = DoubleNan::URX_NAN;

  {
    dataset->acquisition.probes.push_back(std::make_shared<Probe>());

    auto probe = std::make_shared<Probe>();
    probe->description = "Probe 1";
    probe->type = ProbeType::MATRIX;
    probe->transform = {{-1, 2.3, 4e5}, {1.2, -2.2, 232}};
    {
      probe->element_geometries.push_back(std::make_shared<ElementGeometry>());

      auto element_geometry = std::make_shared<ElementGeometry>();
      element_geometry->perimeter = {{15, 23, 543}, {2.2, 1.4, 0.2}};
      probe->element_geometries.push_back(element_geometry);

      element_geometry = std::make_shared<ElementGeometry>();
      element_geometry->perimeter = {{32.2, 31.4, 30.2}, {1.2, 2.4, 543}};
      probe->element_geometries.push_back(element_geometry);
    }
    {
      probe->impulse_responses.push_back(std::make_shared<ImpulseResponse>());

      auto impulse_response = std::make_shared<ImpulseResponse>();
      impulse_response->sampling_frequency = 20000001;
      impulse_response->data = {1.2, 1.3, 1.4};
      impulse_response->units = "meter";
      impulse_response->time_offset = 10000;
      probe->impulse_responses.push_back(impulse_response);

      impulse_response = std::make_shared<ImpulseResponse>();
      impulse_response->sampling_frequency = std::numeric_limits<double>::quiet_NaN();
      impulse_response->data = {2.2, 2.3, 2.4};
      impulse_response->units = "milli";
      impulse_response->time_offset = std::numeric_limits<double>::signaling_NaN();
      probe->impulse_responses.push_back(impulse_response);
    }

    probe->elements.emplace_back();

    {
      Element element;
      element.transform = {{1, 2, 3}, {4e-3, 5e-3, 6e-3}};
      element.element_geometry = probe->element_geometries[0];
      element.impulse_response = probe->impulse_responses[1];
      probe->elements.push_back(std::move(element));
    }

    {
      Element element;
      element.transform = {{-1, -2, -3}, {4e-3, 5e-3, 6e-3}};
      element.element_geometry = probe->element_geometries[1];
      element.impulse_response = probe->impulse_responses[2];
      probe->elements.push_back(std::move(element));
    }

    {
      Element element;
      element.transform = {{1.23, 4.56, 7.89}, {10, -11.1, 12.3}};
      element.element_geometry = probe->element_geometries[1];
      element.impulse_response = probe->impulse_responses[0];
      probe->elements.push_back(std::move(element));
    }

    {
      Element element;
      element.transform = {{-33, 2.2, 44}, {77, 8.8, -97.8e5}};
      element.element_geometry = probe->element_geometries[2];
      element.impulse_response = probe->impulse_responses[1];
      probe->elements.push_back(std::move(element));
    }
    dataset->acquisition.probes.push_back(probe);

    probe = std::make_shared<Probe>();
    probe->description = "Probe 2";
    // NOLINTNEXTLINE
    probe->type = static_cast<ProbeType>(123);
    probe->transform = {{3, 4, 5}, {3.2, 4.2, 5.2}};

    {
      probe->element_geometries.push_back(std::make_shared<ElementGeometry>());

      auto element_geometry = std::make_shared<ElementGeometry>();
      element_geometry->perimeter = {{123, 4.5, 5.4}, {1.1, 22.1, 5.4}};
      probe->element_geometries.push_back(element_geometry);

      element_geometry = std::make_shared<ElementGeometry>();
      element_geometry->perimeter = {{5.5, 6.6, 7.7}, {1.1, 0.0, 5.5}};
      probe->element_geometries.push_back(element_geometry);
    }
    {
      probe->impulse_responses.push_back(std::make_shared<ImpulseResponse>());

      auto impulse_response = std::make_shared<ImpulseResponse>();
      impulse_response->sampling_frequency = 20000011;
      impulse_response->data = {22.2, 3.3, 144.4};
      impulse_response->units = "met55er";
      impulse_response->time_offset = 100500;
      probe->impulse_responses.push_back(impulse_response);

      impulse_response = std::make_shared<ImpulseResponse>();
      impulse_response->sampling_frequency = 5000004402;
      impulse_response->data = {23.2, 2.53, 72.4};
      impulse_response->units = "mi3lli";
      impulse_response->time_offset = 200100;
      probe->impulse_responses.push_back(impulse_response);
    }
    {
      Element element;
      element.transform = {{-99, 8.8, 77}, {74.1, 8.52, -.963}};
      element.element_geometry = probe->element_geometries[1];
      element.impulse_response = probe->impulse_responses[1];
      probe->elements.push_back(std::move(element));
    }

    {
      Element element;
      element.transform = {{3.3, 11, 2.2}, {-86, -75.3, 42.}};
      element.element_geometry = probe->element_geometries[2];
      element.impulse_response = probe->impulse_responses[2];
      probe->elements.push_back(std::move(element));
    }
    dataset->acquisition.probes.push_back(probe);
  }

  {
    dataset->acquisition.excitations.push_back(std::make_shared<ExcitationT>());

    auto excitation = std::make_shared<ExcitationT>();
    excitation->pulse_shape = "sinusoidal";
    excitation->transmit_frequency = 123456789;
    excitation->sampling_frequency = 987654321;
    excitation->waveform = {0, 0.1, 0.2};
    dataset->acquisition.excitations.push_back(excitation);

    excitation = std::make_shared<ExcitationT>();
    excitation->pulse_shape = "square wave";
    excitation->transmit_frequency = 741085296;
    excitation->sampling_frequency = 369258147;
    excitation->waveform = {1, 1.1, 1.2};
    dataset->acquisition.excitations.push_back(excitation);
  }

  {
    dataset->acquisition.groups.push_back(std::make_shared<GroupT>());

    auto group = std::make_shared<GroupT>();
    group->sampling_type = SamplingType::IQ;
    group->data_type = DataType::DOUBLE;
    group->sound_speed = 1500.;
    group->sequence.emplace_back();
    {
      EventT event;
      event.transmit_setup.probe = dataset->acquisition.probes[0];
      event.transmit_setup.probe_transform = {{5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};
      event.transmit_setup.time_offset = 120.2;
      event.transmit_setup.wave.type = WaveType::CONVERGING_WAVE;
      event.transmit_setup.wave.time_zero_reference_point = {5, 4, 3};
      event.transmit_setup.wave.time_zero = 5.11;
      event.transmit_setup.wave.parameters = {7, 53e-12, .2, 1, .3, 5.6e20, 7};
      event.transmit_setup.active_elements = {{}, {0, 1}, {0}};
      event.transmit_setup.excitations = {dataset->acquisition.excitations[1],
                                          dataset->acquisition.excitations[0]};
      event.transmit_setup.delays = {1.2, .3};
      event.receive_setup.probe = dataset->acquisition.probes[1];
      event.receive_setup.probe_transform = {{5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};
      event.receive_setup.sampling_frequency = 125468;
      event.receive_setup.number_samples = 1;
      event.receive_setup.active_elements = {{1, 2, 753, 8, 6, 2, 1}, {}};
      event.receive_setup.tgc_profile = {1.2, 45e120, 4.5, -4.2e-50, 1, .5, 4.6, 1, 2};
      event.receive_setup.tgc_sampling_frequency = 12.5e10;
      event.receive_setup.modulation_frequency = -125;
      event.receive_setup.time_offset = 1205;
      group->sequence.push_back(std::move(event));
    }

    {
      EventT event;
      event.transmit_setup.probe = dataset->acquisition.probes[1];
      event.transmit_setup.probe_transform = {{5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};
      event.transmit_setup.time_offset = 12052;
      event.transmit_setup.wave.type = WaveType::CONVERGING_WAVE;
      event.transmit_setup.wave.time_zero_reference_point.x = 1;
      event.transmit_setup.wave.time_zero_reference_point.y = 2;
      event.transmit_setup.wave.time_zero_reference_point.z = 3;
      event.transmit_setup.wave.time_zero = 4.11;
      event.transmit_setup.wave.parameters = {7, .3, 5.6, 7};
      event.transmit_setup.active_elements = {{1}, {0, 1}};
      event.transmit_setup.excitations = {dataset->acquisition.excitations[1],
                                          dataset->acquisition.excitations[1]};
      event.transmit_setup.delays = {.3, 1.2};
      event.receive_setup.probe = dataset->acquisition.probes[2];
      event.receive_setup.probe_transform = {{5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};
      event.receive_setup.sampling_frequency = 1268;
      event.receive_setup.number_samples = 2;
      event.receive_setup.active_elements = {{1, 2, 3}};
      event.receive_setup.tgc_profile = {1, .2, 4., 5, .1, 5, .45, 5};
      event.receive_setup.tgc_sampling_frequency = 4.5;
      event.receive_setup.modulation_frequency = 4.58;
      event.receive_setup.time_offset = 7.8;
      group->sequence.push_back(std::move(event));
    }
    group->description = "group description";
    dataset->acquisition.groups.push_back(group);

    group = std::make_shared<GroupT>();
    group->sampling_type = SamplingType::RF;
    group->data_type = DataType::INT16;
    group->sound_speed = 1550.;

    group->sequence.emplace_back();
    {
      EventT event;
      event.transmit_setup.probe = dataset->acquisition.probes[1];
      event.transmit_setup.probe_transform = {{5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};
      event.transmit_setup.time_offset = 1202.2;
      event.transmit_setup.wave.type = WaveType::CYLINDRICAL_WAVE;
      event.transmit_setup.wave.time_zero_reference_point.x = 3;
      event.transmit_setup.wave.time_zero_reference_point.y = 4;
      event.transmit_setup.wave.time_zero_reference_point.z = 5;
      event.transmit_setup.wave.time_zero = 5.161;
      event.transmit_setup.wave.parameters = {1, .2, 4, 3, 6, 4.5};
      event.transmit_setup.active_elements = {{0}, {1}};
      event.transmit_setup.excitations = {dataset->acquisition.excitations[0],
                                          dataset->acquisition.excitations[1]};
      event.transmit_setup.delays = {1.2, 45};
      event.receive_setup.probe = dataset->acquisition.probes[0];
      event.receive_setup.probe_transform = {{5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};
      event.receive_setup.sampling_frequency = 1275468;
      event.receive_setup.number_samples = 2;
      event.receive_setup.active_elements = {{1, 2, 5753, 58, 60, 2, 1}};
      event.receive_setup.tgc_profile = {1.2, 45., 4.25, -4.52, 1, .5, 4.36, 1, 2};
      event.receive_setup.tgc_sampling_frequency = -12.5;
      event.receive_setup.modulation_frequency = 1925;
      event.receive_setup.time_offset = 206;
      group->sequence.push_back(std::move(event));
    }

    {
      EventT event;
      event.transmit_setup.probe = dataset->acquisition.probes[0];
      event.transmit_setup.probe_transform = {{5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};
      event.transmit_setup.time_offset = 1202;
      event.transmit_setup.wave.type = WaveType::CONVERGING_WAVE;
      event.transmit_setup.wave.time_zero_reference_point = {6, 5, 2};
      event.transmit_setup.wave.time_zero = 4.11;
      event.transmit_setup.wave.parameters = {7, 53, .3, 5.6, 7};
      event.transmit_setup.active_elements = {{1}};
      event.transmit_setup.excitations = {dataset->acquisition.excitations[0]};
      event.transmit_setup.delays = {1.6};
      event.receive_setup.probe = dataset->acquisition.probes[2];
      event.receive_setup.probe_transform = {{5.1e-120, -8, 7.}, {5.2, 4.3, 8.2e10}};
      event.receive_setup.sampling_frequency = 12468;
      event.receive_setup.number_samples = 2;
      event.receive_setup.active_elements = {{15, 32, 3}};
      event.receive_setup.tgc_profile = {17, .2, 3., 5, .1, 35, .445, 5};
      event.receive_setup.tgc_sampling_frequency = 47.5;
      event.receive_setup.modulation_frequency = 4.558;
      event.receive_setup.time_offset = 7.38;
      group->sequence.push_back(std::move(event));
    }
    group->description = "group2 description";
    dataset->acquisition.groups.push_back(group);
    dataset->acquisition.groups.push_back(std::make_shared<GroupT>());
    dataset->acquisition.groups.back()->sequence.emplace_back();
  }

  dataset->acquisition.timestamp = 1242;

  if constexpr (std::is_same_v<T, urx::Dataset>) {
    using GroupDataT = typename decltype(T::acquisition.groups_data)::value_type;
    {
      dataset->acquisition.groups_data.emplace_back();
      {
        GroupDataT group_data;
        group_data.group = dataset->acquisition.groups[2];

        group_data.raw_data = std::make_shared<RawDataNoInit<double>>(6);
        auto* buf = static_cast<double*>(group_data.raw_data->getBuffer());
        buf[0] = 1.2;
        buf[1] = 2.3;
        buf[2] = 3.4;
        buf[3] = 4.5;
        buf[4] = 5.6;
        buf[5] = 6.7;

        group_data.group_timestamp = 283954.334;
        group_data.sequence_timestamps = {1, 2, 4.2, 1, .5, 5.6};
        group_data.event_timestamps = {{1, .24, 1., 5.2, 4.5, 7, .964, .5},
                                       {1, 2, 4, 85, .15, -4.5, -7, .45, .6, 4}};
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.group = dataset->acquisition.groups[1];

        group_data.raw_data = std::make_shared<RawDataNoInit<std::complex<int16_t>>>(4);
        auto* buf = static_cast<std::complex<int16_t>*>(group_data.raw_data->getBuffer());
        buf[0] = {123, 456};
        buf[1] = {159, 753};
        buf[2] = {789, 456};
        buf[3] = {123, 753};
        group_data.group_timestamp = 123;
        group_data.sequence_timestamps = {1, 2, 34};
        group_data.event_timestamps = {{4, 5, 7}, {8, 7, 6}};
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.group = dataset->acquisition.groups[1];

        std::vector<std::complex<int16_t>> buf = {
            {123, 456}, {159, 753}, {789, 456}, {123, 753}, {-255, 15909}};

        group_data.raw_data =
            std::make_shared<RawDataVector<std::complex<int16_t>>>(std::move(buf));
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.group = dataset->acquisition.groups[0];

        group_data.raw_data = std::make_shared<RawDataWeak<int32_t>>(nullptr, 0);
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.raw_data = std::make_shared<RawDataNoInit<int16_t>>(1);
        auto* buf = static_cast<int16_t*>(group_data.raw_data->getBuffer());
        buf[0] = 123;
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.raw_data = std::make_shared<RawDataNoInit<int32_t>>(1);
        auto* buf = static_cast<int32_t*>(group_data.raw_data->getBuffer());
        buf[0] = 2345678;
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.raw_data = std::make_shared<RawDataNoInit<float>>(1);
        auto* buf = static_cast<float*>(group_data.raw_data->getBuffer());
        buf[0] = 2345.678f;
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.raw_data = std::make_shared<RawDataNoInit<double>>(1);
        auto* buf = static_cast<double*>(group_data.raw_data->getBuffer());
        buf[0] = 234567.8;
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.raw_data = std::make_shared<RawDataNoInit<std::complex<int16_t>>>(1);
        auto* buf = static_cast<std::complex<int16_t>*>(group_data.raw_data->getBuffer());
        buf[0] = {123, -123};
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.raw_data = std::make_shared<RawDataNoInit<std::complex<int32_t>>>(1);
        auto* buf = static_cast<std::complex<int32_t>*>(group_data.raw_data->getBuffer());
        buf[0] = {2345678, -2345678};
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.raw_data = std::make_shared<RawDataNoInit<std::complex<float>>>(1);
        auto* buf = static_cast<std::complex<float>*>(group_data.raw_data->getBuffer());
        buf[0] = {2.f, 345.678f};
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
      {
        GroupDataT group_data;
        group_data.raw_data = std::make_shared<RawDataNoInit<std::complex<double>>>(1);
        auto* buf = static_cast<std::complex<double>*>(group_data.raw_data->getBuffer());
        buf[0] = {234., 567.8};
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
    }
  }

  return dataset;
}

template <typename T>
std::shared_ptr<T> generateWrongDataset() {
  auto dataset = generateFakeDataset<T>();
  auto dataset_bis = generateFakeDataset<T>();

  auto wrong_probe = std::make_shared<Probe>();
  wrong_probe->description = "Probe with wrong ptr";

  {
    wrong_probe->element_geometries.push_back(std::make_shared<ElementGeometry>());
    wrong_probe->element_geometries.resize(2);
  }
  {
    wrong_probe->impulse_responses.push_back(std::make_shared<ImpulseResponse>());
    wrong_probe->impulse_responses.resize(2);
  }
  wrong_probe->elements.emplace_back();
  {
    Element element;
    element.element_geometry = wrong_probe->element_geometries[0];
    element.impulse_response = wrong_probe->impulse_responses[0];
    wrong_probe->elements.push_back(std::move(element));
  }

  {
    Element element;
    element.element_geometry = dataset->acquisition.probes.back()->element_geometries[0];
    element.impulse_response = dataset->acquisition.probes.back()->impulse_responses[0];
    wrong_probe->elements.push_back(std::move(element));
  }

  {
    Element element;
    element.element_geometry = dataset->acquisition.probes.back()->element_geometries[2];
    element.impulse_response = dataset->acquisition.probes.back()->impulse_responses[2];
    wrong_probe->elements.push_back(std::move(element));
  }

  {
    Element element;
    element.element_geometry = dataset_bis->acquisition.probes.back()->element_geometries[2];
    element.impulse_response = dataset_bis->acquisition.probes.back()->impulse_responses[2];
    wrong_probe->elements.push_back(std::move(element));
  }
  dataset->acquisition.probes.push_back(wrong_probe);

  for (size_t g_id = 0; g_id < dataset->acquisition.groups.size(); ++g_id) {
    for (size_t e_id = 0; e_id < dataset->acquisition.groups.at(g_id)->sequence.size(); ++e_id) {
      auto& rs_active_elements =
          dataset->acquisition.groups.at(g_id)->sequence.at(e_id).receive_setup.active_elements;
      rs_active_elements.resize(rs_active_elements.size() + 1);

      auto& ts_active_elements =
          dataset->acquisition.groups.at(g_id)->sequence.at(e_id).transmit_setup.active_elements;
      ts_active_elements.resize(ts_active_elements.size() + 1);

      auto& ex_vec =
          dataset->acquisition.groups.at(g_id)->sequence.at(e_id).transmit_setup.excitations;
      ex_vec.emplace_back();
      ex_vec.push_back(dataset_bis->acquisition.excitations.back());
      ex_vec.resize(ex_vec.size() + 1);
    }
    if (!dataset->acquisition.groups.at(g_id)->sequence.empty()) {
      auto event = dataset->acquisition.groups.at(g_id)->sequence.back();
      event.transmit_setup.probe = dataset_bis->acquisition.probes.back();
      event.receive_setup.probe = dataset_bis->acquisition.probes.back();
      dataset->acquisition.groups.at(g_id)->sequence.push_back(event);
    }
  }
  if constexpr (std::is_same_v<T, urx::Acquisition>) {
    using GroupDataT = typename decltype(T::acquisition.groups_data)::value_type;
    {
      {
        GroupDataT group_data;
        group_data.group = dataset_bis->acquisition.groups[1];

        group_data.raw_data = std::make_shared<RawDataNoInit<double>>(6);
        auto* buf = static_cast<double*>(group_data.raw_data->getBuffer());
        buf[0] = 1.2;
        buf[1] = 2.3;
        buf[2] = 3.4;
        buf[3] = 4.5;
        buf[4] = 5.6;
        buf[5] = 6.7;

        group_data.group_timestamp = 283954.334;
        group_data.sequence_timestamps = {1, 2, 4.2, 1, .5, 5.6};
        group_data.event_timestamps = {{1, .24, 1., 5.2, 4.5, 7, .964, .5},
                                       {1, 2, 4, 85, .15, -4.5, -7, .45, .6, 4}};
        dataset->acquisition.groups_data.push_back(std::move(group_data));
      }
    }
    dataset->acquisition.groups_data.resize(dataset->acquisition.groups_data.size() + 1);
  }

  dataset->acquisition.groups.resize(dataset->acquisition.groups.size() + 1);
  dataset->acquisition.excitations.resize(dataset->acquisition.excitations.size() + 1);
  dataset->acquisition.probes.resize(dataset->acquisition.probes.size() + 1);

  return dataset;
}

}  // namespace urx::utils::test
