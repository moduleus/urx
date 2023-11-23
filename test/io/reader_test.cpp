#include <uff/dataset.h>
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace uff::test {

TEST_CASE("Load HDF5 file", "[hdf5_loader]") {
  /*
  constexpr uint8_t NB_CHANNELS = 32;
  constexpr uint8_t NB_SAMPLES = 10;
  constexpr uint8_t NB_FRAMES = 1;
  constexpr uint8_t NB_PW = 1;
  */

  auto dataset = std::make_shared<uff::Dataset>();

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
