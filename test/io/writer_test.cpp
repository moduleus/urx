#include <complex>
#include <cstdlib>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <uff/acquisition.h>
#include <uff/dataset.h>
#include <uff/detail/compare.h>
#include <uff/detail/double_nan.h>
#include <uff/detail/raw_data.h>
#include <uff/element.h>
#include <uff/element_geometry.h>
#include <uff/event.h>
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/impulse_response.h>
#include <uff/probe.h>
#include <uff/receive_setup.h>
#include <uff/transform.h>
#include <uff/transmit_setup.h>
#include <uff/vector.h>
#include <uff/version.h>
#include <uff/wave.h>
#include <uff_utils/io/reader.h>
#include <uff_utils/io/writer.h>

namespace uff::test {

TEST_CASE("Write HDF5 file", "[hdf5_writer]") {
  auto dataset = std::make_shared<uff::Dataset>();

  dataset->version.major = 123;
  dataset->version.minor = 456;
  dataset->version.patch = 789;

  dataset->acquisition.authors = "AuthOr";
  dataset->acquisition.description = "Description";
  dataset->acquisition.local_time = "2019-06-15T15:53:00";
  dataset->acquisition.country_code = "FR";
  dataset->acquisition.system = "???";
  dataset->acquisition.sound_speed = 1500.;

  {
    auto probe = std::make_shared<Probe>();
    probe->description = "Probe 1";
    probe->type = uff::Probe::ProbeType::MATRIX;
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
    probe->type = uff::Probe::ProbeType::SPARSE;
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
    wave->type = uff::Wave::WaveType::CYLINDRICAL_WAVE;
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
    wave->type = uff::Wave::WaveType::CONVERGING_WAVE;
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
    auto group = std::make_shared<uff::Group>();
    group->sampling_type = uff::Group::SamplingType::IQ;
    group->data_type = uff::Group::DataType::INT16;
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

    group = std::make_shared<uff::Group>();
    group->sampling_type = uff::Group::SamplingType::RF;
    group->data_type = uff::Group::DataType::DOUBLE;
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
    auto group_data = std::make_shared<GroupData>();
    group_data->group = dataset->acquisition.groups[1];
    group_data->raw_data.size = 6;
    const std::shared_ptr<double[]> buffer = std::shared_ptr<double[]>(
        static_cast<double*>(malloc(sizeof(double) * group_data->raw_data.size)), free);
    buffer[0] = 1.2;
    buffer[1] = 2.3;
    buffer[2] = 3.4;
    buffer[3] = 4.5;
    buffer[4] = 5.6;
    buffer[5] = 6.7;
    group_data->raw_data.buffer = buffer;

    group_data->group_timestamp = 283954.334;
    group_data->sequence_timestamps = {1, 2, 4.2, 1, .5, 5.6};
    group_data->event_timestamps = {{1, .24, 1., 5.2, 4.5, 7, .964, .5},
                                    {1, 2, 4, 85, .15, -4.5, -7, .45, .6, 4}};
    dataset->acquisition.groups_data.push_back(group_data);

    group_data = std::make_shared<GroupData>();
    group_data->group = dataset->acquisition.groups[0];
    group_data->raw_data.size = 4;
    const std::shared_ptr<std::complex<short>[]> buffer2 = std::shared_ptr<std::complex<short>[]>(
        static_cast<std::complex<short>*>(
            malloc(sizeof(std::complex<short>) * group_data->raw_data.size)),
        free);
    buffer2[0] = {123, 456};
    buffer2[1] = {159, 753};
    buffer2[2] = {789, 456};
    buffer2[3] = {123, 753};
    group_data->raw_data.buffer = buffer2;
    group_data->group_timestamp = 123;
    group_data->sequence_timestamps = {1, 2, 34};
    group_data->event_timestamps = {{4, 5, 7}, {8, 7, 6}};
    dataset->acquisition.groups_data.push_back(group_data);
  }

  uff::Writer::saveToFile("writer.uff", *dataset);

  auto dataset_loaded = uff::Reader::loadFromFile("writer.uff");

  REQUIRE(dataset_loaded->acquisition.probes == dataset->acquisition.probes);
  REQUIRE(dataset_loaded->acquisition.excitations == dataset->acquisition.excitations);
  REQUIRE(dataset_loaded->acquisition.waves == dataset->acquisition.waves);
  REQUIRE(dataset_loaded->acquisition.groups == dataset->acquisition.groups);
  REQUIRE(dataset_loaded->acquisition.groups_data == dataset->acquisition.groups_data);
  REQUIRE(*dataset_loaded == *dataset);

  // uff::ChannelData<float>& channelData = dataset->channelData();
  // channelData.setAuthors("Unknown");
  // channelData.setDescription("Missing description");
  // channelData.setLocalTime("1970-01-01T00:00:00");
  // channelData.setCountryCode("FR");
  // channelData.setSystem("Unknown");
  // channelData.setRepetitionRate(1.0);
  // channelData.setSoundSpeed(1540.);

  // channelData.setNumberOfFrames(NB_FRAMES);
  // channelData.setNumberOfEvents(NB_PW);
  // channelData.setNumberOfChannels(NB_CHANNELS);
  // channelData.setNumberOfSamples(NB_SAMPLES);
  // channelData.allocate();
  // std::vector<float> dataVec(320);
  // short maxLimit = std::numeric_limits<short>::max();
  // for (auto& data : dataVec) {
  //   data = static_cast<short>(std::rand() / (RAND_MAX / maxLimit));
  // }
  // channelData.setData(dataVec);

  // // Probe RCA
  // // RCA made of 2 linear array
  // auto probe = std::make_shared<uff::RcaArray>(channelData.numberOfChannels(),
  //                                              channelData.numberOfChannels());
  // double pitch_x = 0.42;
  // probe->setPitchX(pitch_x);
  // double pitch_y = 0.42;
  // probe->setPitchY(pitch_y);
  // channelData.addProbe(probe);

  // // Channel mapping
  // std::vector<int> mapping_x(NB_CHANNELS);
  // std::vector<int> mapping_y(NB_CHANNELS);
  // for (int i = 0; i < NB_CHANNELS; ++i) {
  //   mapping_x[i] = i;
  //   mapping_y[i] = i + NB_CHANNELS;
  // }

  // // Receive setup
  // uff::ReceiveSetup receiveSetup_y;
  // receiveSetup_y.setProbe(probe);
  // receiveSetup_y.setTimeOffset(0);
  // receiveSetup_y.setSamplingFrequency(dataset->samplingFrequency());
  // receiveSetup_y.setChannelMapping(mapping_y);

  // std::vector<uff::Event> sequence;

  // // Plane waves
  // for (int pw = 0; pw < NB_PW; ++pw) {
  //   // plane wave. origin.translation is the direction vector
  //   auto wave = std::make_shared<uff::Wave>();
  //   wave->setWaveType(uff::WaveType::PLANE_WAVE);
  //   wave->setOrigin(uff::Transform(uff::Rotation(), uff::Vector3D<double>()));

  //   uff::Excitation excitation;
  //   excitation.setTransmitFrequency(dataset->transmitFrequency());
  //   wave->setExcitation(excitation);
  //   channelData.addUniqueWave(wave);

  //   uff::TransmitWave transmitWave;
  //   transmitWave.setWave(wave);
  //   transmitWave.setTimeOffset(0.);

  //   uff::TransmitSetup transmitSetup;
  //   transmitSetup.setProbe(probe);
  //   transmitSetup.setTransmitWave(transmitWave);

  //   auto event = std::make_shared<uff::Event>();
  //   event->setTransmitSetup(transmitSetup);
  //   event->setReceiveSetup(receiveSetup_y);
  //   channelData.addUniqueEvent(event);

  //   // plane-waves have 125us delay
  //   uff::Event Event;
  //   Event.setEvent(event);
  //   Event.setTimeOffset(pw * 125e-6);

  //   // Enqueue the plane-wave
  //   sequence.push_back(Event);
  // }

  // channelData.setSequence(sequence);

  // dataset->setChannelData(channelData);

  // std::string filename = "tmp.uff";
  // uff::Writer<float> floatWriter;
  // uff::Reader<float> floatReader;

  // floatWriter.setFileName(filename);
  // floatWriter.setDataset(dataset);
  // floatWriter.writeToFile();

  // std::shared_ptr<uff::Dataset<short>> readShortDataset =
  //     uff::Reader<short>::loadFile(filename, true, false);
  // floatReader.setFileName(filename);
  // floatReader.updateMetadata();
  // std::shared_ptr<uff::Dataset<float>> readFloatDataset = floatReader.dataset();

  // const std::vector<short>& shortVec = readShortDataset->channelData().data();
  // const std::vector<float>& floatVec = readFloatDataset->channelData().data();
  // REQUIRE(std::equal(shortVec.begin(), shortVec.end(), floatVec.begin()));

  // REQUIRE(readShortDataset->channelData().authors() == channelData.authors());
  // REQUIRE(readShortDataset->channelData().description() == channelData.description());
  // REQUIRE(readShortDataset->channelData().localTime() == channelData.localTime());
  // REQUIRE(readShortDataset->channelData().countryCode() == channelData.countryCode());
  // REQUIRE(readShortDataset->channelData().system() == channelData.system());
  // REQUIRE(readShortDataset->channelData().repetitionRate() == channelData.repetitionRate());
  // REQUIRE(readShortDataset->soundSpeed() == channelData.soundSpeed());
  // {
  //   std::vector<double> geometry1 = readShortDataset->channelGeometry<double>();
  //   std::vector<double> geometry2 = channelData.channelGeometry<double>();
  //   REQUIRE(geometry1.size() == 4UL * 2 * NB_CHANNELS);
  //   REQUIRE(geometry2.size() == 4UL * 2 * NB_CHANNELS);
  //   REQUIRE(std::equal(geometry1.begin(), geometry1.end(), geometry2.begin()));
  // }
  // REQUIRE(readShortDataset->receiveDelay() == channelData.receiveDelay());
  // REQUIRE(readShortDataset->samplingType() == channelData.samplingType());
  // REQUIRE(std::isnan(readShortDataset->samplingFrequency()));
  // REQUIRE(std::isnan(channelData.samplingFrequency()));
  // REQUIRE(std::isnan(readShortDataset->transmitFrequency()));
  // REQUIRE(std::isnan(channelData.transmitFrequency()));
  // REQUIRE(readShortDataset->isProbeType<uff::RcaArray>());
  // REQUIRE(channelData.isProbeType<uff::RcaArray>());
  // REQUIRE(!readShortDataset->isProbeType<uff::MatrixArray>());
  // REQUIRE(!channelData.isProbeType<uff::MatrixArray>());
  // REQUIRE(!readShortDataset->isProbeType<uff::LinearArray>());
  // REQUIRE(!channelData.isProbeType<uff::LinearArray>());
}

}  // namespace uff::test
