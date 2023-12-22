#include <complex>
#include <limits>
#include <memory>
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
#include <urx/utils/io/reader.h>
#include <urx/utils/io/writer.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx::test {

TEST_CASE("Write HDF5 file", "[hdf5_writer]") {
  auto dataset = std::make_shared<urx::Dataset>();

  dataset->version.major = 123;
  dataset->version.minor = 456;
  dataset->version.patch = 789;

  dataset->acquisition.authors = "AuthOr";
  dataset->acquisition.description = "康Description🐜";
  dataset->acquisition.local_time = "2019-06-15T15:53:00";
  dataset->acquisition.country_code = "FR";
  dataset->acquisition.system = "???";
  dataset->acquisition.sound_speed = 1500.;

  {
    auto probe = std::make_shared<Probe>();
    probe->description = "Probe 1";
    probe->type = urx::Probe::ProbeType::MATRIX;
    probe->transform.rotation.x = 1;
    probe->transform.rotation.y = 2;
    probe->transform.rotation.z = 3;
    probe->transform.translation.x = 1.2;
    probe->transform.translation.y = 2.2;
    probe->transform.translation.z = 3.2;
    {
      auto element_geometry = std::make_shared<ElementGeometry>();
      element_geometry->perimeter = {{.x = 15, .y = 23, .z = 543}, {.x = 2.2, .y = 1.4, .z = 0.2}};
      probe->element_geometries.push_back(element_geometry);

      element_geometry = std::make_shared<ElementGeometry>();
      element_geometry->perimeter = {{.x = 2.2, .y = 1.4, .z = 0.2},
                                     {.x = 1.2, .y = 2.4, .z = 543}};
      probe->element_geometries.push_back(element_geometry);
    }
    {
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
    {
      Element element;
      element.transform.rotation.x = 11;
      element.transform.rotation.y = 22;
      element.transform.rotation.z = 33;
      element.transform.translation.x = 44155;
      element.transform.translation.y = 44255;
      element.transform.translation.z = 44355;
      element.element_geometry = probe->element_geometries[0];
      element.impulse_response = probe->impulse_responses[1];
      probe->elements.push_back(std::move(element));
    }

    {
      Element element;
      element.transform.rotation.x = 33;
      element.transform.rotation.y = 22;
      element.transform.rotation.z = 44;
      element.transform.translation.x = 77;
      element.transform.translation.y = 88;
      element.transform.translation.z = 978;
      element.element_geometry = probe->element_geometries[1];
      element.impulse_response = probe->impulse_responses[0];
      probe->elements.push_back(std::move(element));
    }
    dataset->acquisition.probes.push_back(probe);

    probe = std::make_shared<Probe>();
    probe->description = "Probe 2";
    probe->type = static_cast<urx::Probe::ProbeType>(123);
    probe->transform.rotation.x = 3;
    probe->transform.rotation.y = 4;
    probe->transform.rotation.z = 5;
    probe->transform.translation.x = 3.2;
    probe->transform.translation.y = 4.2;
    probe->transform.translation.z = 5.2;
    {
      auto element_geometry = std::make_shared<ElementGeometry>();
      element_geometry->perimeter = {{.x = 123, .y = 4.5, .z = 5.4},
                                     {.x = 1.1, .y = 22.1, .z = 5.4}};
      probe->element_geometries.push_back(element_geometry);

      element_geometry = std::make_shared<ElementGeometry>();
      element_geometry->perimeter = {{.x = 5.5, .y = 6.6, .z = 7.7},
                                     {.x = 1.1, .y = 0.0, .z = 5.5}};
      probe->element_geometries.push_back(element_geometry);
    }
    {
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
      element.transform.rotation.x = 99;
      element.transform.rotation.y = 88;
      element.transform.rotation.z = 77;
      element.transform.translation.x = 741;
      element.transform.translation.y = 852;
      element.transform.translation.z = 963;
      element.element_geometry = probe->element_geometries[1];
      element.impulse_response = probe->impulse_responses[1];
      probe->elements.push_back(std::move(element));
    }

    {
      Element element;
      element.transform.rotation.x = 33;
      element.transform.rotation.y = 11;
      element.transform.rotation.z = 22;
      element.transform.translation.x = 86;
      element.transform.translation.y = 753;
      element.transform.translation.z = 42;
      element.element_geometry = probe->element_geometries[0];
      element.impulse_response = probe->impulse_responses[0];
      probe->elements.push_back(std::move(element));
    }
    dataset->acquisition.probes.push_back(probe);
  }

  {
    auto excitation = std::make_shared<Excitation>();
    excitation->pulse_shape = "sinusoidal";
    excitation->transmit_frequency = 123456789;
    excitation->sampling_frequency = 987654321;
    excitation->waveform = {0, 0.1, 0.2};
    dataset->acquisition.excitations.push_back(excitation);

    excitation = std::make_shared<Excitation>();
    excitation->pulse_shape = "square wave";
    excitation->transmit_frequency = 741085296;
    excitation->sampling_frequency = 369258147;
    excitation->waveform = {1, 1.1, 1.2};
    dataset->acquisition.excitations.push_back(excitation);
  }

  {
    auto wave = std::make_shared<Wave>();
    wave->type = urx::Wave::WaveType::CYLINDRICAL_WAVE;
    wave->time_zero_reference_point.x = 3;
    wave->time_zero_reference_point.y = 4;
    wave->time_zero_reference_point.z = 5;
    wave->time_zero = 5.11;
    wave->channel_mapping = {{1, 2, 3, 45, 9, 7, 5},
                             {1, 2, 5, 7, 58, 961, 5, 5, 7, 7, 8, 5},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12}};
    wave->channel_excitations = {dataset->acquisition.excitations[0],
                                 dataset->acquisition.excitations[1]};
    wave->channel_delays = {1.2, 45, .26, 1.2, 5};
    wave->parameters = {1, .2, 4, 3, 6, 4.5};
    dataset->acquisition.waves.push_back(wave);

    wave = std::make_shared<Wave>();
    wave->type = urx::Wave::WaveType::CONVERGING_WAVE;
    wave->time_zero_reference_point.x = 5;
    wave->time_zero_reference_point.y = 4;
    wave->time_zero_reference_point.z = 4;
    wave->time_zero = 5.11;
    wave->channel_mapping = {{45, 4, 2, 5, 54, 53, 5, 5, 66},
                             {5, 7, 25, 1, 3, 5, 245, 6, 42},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12},
                             {12, 12}};
    wave->channel_excitations = {dataset->acquisition.excitations[1],
                                 dataset->acquisition.excitations[0]};
    wave->channel_delays = {1.2, .3, .5, 10.4};
    wave->parameters = {7, 53, .2, 1, .3, 5.6, 7};
    dataset->acquisition.waves.push_back(wave);
  }

  {
    auto group = std::make_shared<urx::Group>();
    group->sampling_type = urx::Group::SamplingType::IQ;
    group->data_type = urx::Group::DataType::INT16;
    {
      Event event;
      event.transmit_setup.probe = dataset->acquisition.probes[0];
      event.transmit_setup.probe_transform.rotation.x = 5;
      event.transmit_setup.probe_transform.rotation.y = 8;
      event.transmit_setup.probe_transform.rotation.z = 7;
      event.transmit_setup.probe_transform.translation.x = 5.2;
      event.transmit_setup.probe_transform.translation.y = 4.3;
      event.transmit_setup.probe_transform.translation.z = 8.2;
      event.transmit_setup.time_offset = 120.2;
      event.transmit_setup.wave = dataset->acquisition.waves[1];
      event.receive_setup.probe = dataset->acquisition.probes[1];
      event.receive_setup.probe_transform.rotation.x = 51;
      event.receive_setup.probe_transform.rotation.y = 85;
      event.receive_setup.probe_transform.rotation.z = 73;
      event.receive_setup.probe_transform.translation.x = 5.72;
      event.receive_setup.probe_transform.translation.y = 4.83;
      event.receive_setup.probe_transform.translation.z = 8.52;
      event.receive_setup.sampling_frequency = 125468;
      event.receive_setup.number_samples = 4452;
      event.receive_setup.channel_mapping = {
          {1, 2, 753, 8, 6, 2, 1}, {4, 5, 7, 6, 21, 8, 9, 3}, {1, 2, 5, 7, 3, 6, 5}};
      event.receive_setup.tgc_profile = {1.2, 45., 4.5, -4.2, 1, .5, 4.6, 1, 2};
      event.receive_setup.tgc_sampling_frequency = 12.5;
      event.receive_setup.modulation_frequency = 125;
      event.receive_setup.time_offset = 1205;
      group->sequence.push_back(std::move(event));
    }

    {
      Event event;
      event.transmit_setup.probe = dataset->acquisition.probes[1];
      event.transmit_setup.probe_transform.rotation.x = 55;
      event.transmit_setup.probe_transform.rotation.y = 48;
      event.transmit_setup.probe_transform.rotation.z = 77;
      event.transmit_setup.probe_transform.translation.x = 552;
      event.transmit_setup.probe_transform.translation.y = 463;
      event.transmit_setup.probe_transform.translation.z = 872;
      event.transmit_setup.time_offset = 12052;
      event.transmit_setup.wave = dataset->acquisition.waves[0];
      event.receive_setup.probe = dataset->acquisition.probes[0];
      event.receive_setup.probe_transform.rotation.x = 5;
      event.receive_setup.probe_transform.rotation.y = 8;
      event.receive_setup.probe_transform.rotation.z = 3;
      event.receive_setup.probe_transform.translation.x = 52;
      event.receive_setup.probe_transform.translation.y = 43;
      event.receive_setup.probe_transform.translation.z = 52;
      event.receive_setup.sampling_frequency = 1268;
      event.receive_setup.number_samples = 42;
      event.receive_setup.channel_mapping = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
      event.receive_setup.tgc_profile = {1, .2, 4., 5, .1, 5, .45, 5};
      event.receive_setup.tgc_sampling_frequency = 4.5;
      event.receive_setup.modulation_frequency = 4.58;
      event.receive_setup.time_offset = 7.8;
      group->sequence.push_back(std::move(event));
    }
    group->description = "group description";
    dataset->acquisition.groups.push_back(group);

    group = std::make_shared<urx::Group>();
    group->sampling_type = urx::Group::SamplingType::RF;
    group->data_type = urx::Group::DataType::DOUBLE;
    {
      Event event;
      event.transmit_setup.probe = dataset->acquisition.probes[1];
      event.transmit_setup.probe_transform.rotation.x = 55;
      event.transmit_setup.probe_transform.rotation.y = 88;
      event.transmit_setup.probe_transform.rotation.z = 67;
      event.transmit_setup.probe_transform.translation.x = 5.72;
      event.transmit_setup.probe_transform.translation.y = 4.83;
      event.transmit_setup.probe_transform.translation.z = 8.52;
      event.transmit_setup.time_offset = 1202.2;
      event.transmit_setup.wave = dataset->acquisition.waves[0];
      event.receive_setup.probe = dataset->acquisition.probes[0];
      event.receive_setup.probe_transform.rotation.x = 5101;
      event.receive_setup.probe_transform.rotation.y = 825;
      event.receive_setup.probe_transform.rotation.z = 733;
      event.receive_setup.probe_transform.translation.x = 54.72;
      event.receive_setup.probe_transform.translation.y = 45.83;
      event.receive_setup.probe_transform.translation.z = 86.52;
      event.receive_setup.sampling_frequency = 1275468;
      event.receive_setup.number_samples = 48452;
      event.receive_setup.channel_mapping = {
          {1, 2, 5753, 58, 60, 2, 1}, {4, 57, 7, 76, 21, 87, 9, 3}, {17, 26, 57, 7, 8, 3, 6, 5}};
      event.receive_setup.tgc_profile = {01.2, 45., 4.25, -4.52, 1, .5, 4.36, 1, 2};
      event.receive_setup.tgc_sampling_frequency = -12.5;
      event.receive_setup.modulation_frequency = 1925;
      event.receive_setup.time_offset = 1 + 205;
      group->sequence.push_back(std::move(event));
    }

    {
      Event event;
      event.transmit_setup.probe = dataset->acquisition.probes[0];
      event.transmit_setup.probe_transform.rotation.x = 5;
      event.transmit_setup.probe_transform.rotation.y = 8;
      event.transmit_setup.probe_transform.rotation.z = 7;
      event.transmit_setup.probe_transform.translation.x = 52;
      event.transmit_setup.probe_transform.translation.y = 43;
      event.transmit_setup.probe_transform.translation.z = 82;
      event.transmit_setup.time_offset = 1202;
      event.transmit_setup.wave = dataset->acquisition.waves[1];
      event.receive_setup.probe = dataset->acquisition.probes[1];
      event.receive_setup.probe_transform.rotation.x = 55;
      event.receive_setup.probe_transform.rotation.y = 38;
      event.receive_setup.probe_transform.rotation.z = 33;
      event.receive_setup.probe_transform.translation.x = 2;
      event.receive_setup.probe_transform.translation.y = 4;
      event.receive_setup.probe_transform.translation.z = 2;
      event.receive_setup.sampling_frequency = 12468;
      event.receive_setup.number_samples = 442;
      event.receive_setup.channel_mapping = {{15, 32, 3}, {4, 55, 6}, {7, 48, 9}};
      event.receive_setup.tgc_profile = {17, .2, 3., 5, .1, 35, .445, 5};
      event.receive_setup.tgc_sampling_frequency = 47.5;
      event.receive_setup.modulation_frequency = 4.558;
      event.receive_setup.time_offset = 7.38;
      group->sequence.push_back(std::move(event));
    }
    group->description = "group2 description";
    dataset->acquisition.groups.push_back(group);
  }

  dataset->acquisition.timestamp = 1242;

  {
    {
      auto group_data = std::make_shared<GroupData>();
      group_data->group = dataset->acquisition.groups[1];

      group_data->raw_data = std::make_shared<RawDataNoInit<double>>(6);
      double* buf = static_cast<double*>(group_data->raw_data->getBuffer());
      buf[0] = 1.2;
      buf[1] = 2.3;
      buf[2] = 3.4;
      buf[3] = 4.5;
      buf[4] = 5.6;
      buf[5] = 6.7;

      group_data->group_timestamp = 283954.334;
      group_data->sequence_timestamps = {1, 2, 4.2, 1, .5, 5.6};
      group_data->event_timestamps = {{1, .24, 1., 5.2, 4.5, 7, .964, .5},
                                      {1, 2, 4, 85, .15, -4.5, -7, .45, .6, 4}};
      dataset->acquisition.groups_data.push_back(group_data);
    }
    {
      auto group_data = std::make_shared<GroupData>();
      group_data->group = dataset->acquisition.groups[0];

      group_data->raw_data = std::make_shared<RawDataNoInit<std::complex<short>>>(4);
      std::complex<short>* buf =
          static_cast<std::complex<short>*>(group_data->raw_data->getBuffer());
      buf[0] = {123, 456};
      buf[1] = {159, 753};
      buf[2] = {789, 456};
      buf[3] = {123, 753};
      group_data->group_timestamp = 123;
      group_data->sequence_timestamps = {1, 2, 34};
      group_data->event_timestamps = {{4, 5, 7}, {8, 7, 6}};
      dataset->acquisition.groups_data.push_back(group_data);
    }
  }

  urx::Writer::saveToFile("writer康🐜.urx", *dataset);

  auto dataset_loaded = urx::Reader::loadFromFile("writer康🐜.urx");

  REQUIRE(dataset_loaded->acquisition.probes == dataset->acquisition.probes);
  REQUIRE(dataset_loaded->acquisition.excitations == dataset->acquisition.excitations);
  REQUIRE(dataset_loaded->acquisition.waves[0]->channel_mapping ==
          dataset->acquisition.waves[0]->channel_mapping);
  REQUIRE(dataset_loaded->acquisition.waves[0] == dataset->acquisition.waves[0]);
  REQUIRE(dataset_loaded->acquisition.waves[0] == dataset->acquisition.waves[0]);
  REQUIRE(dataset_loaded->acquisition.waves == dataset->acquisition.waves);
  REQUIRE(dataset_loaded->acquisition.groups == dataset->acquisition.groups);
  REQUIRE(dataset_loaded->acquisition.groups_data == dataset->acquisition.groups_data);
  REQUIRE(*dataset_loaded == *dataset);
}

}  // namespace urx::test
