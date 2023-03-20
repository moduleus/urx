#include <uff/channel_data.h>
#include <uff/dataset.h>
#include <uff/event.h>
#include <uff/excitation.h>
#include <uff/rca_array.h>
#include <uff/reader.h>
#include <uff/receive_setup.h>
#include <uff/rotation.h>
#include <uff/timed_event.h>
#include <uff/transform.h>
#include <uff/translation.h>
#include <uff/transmit_setup.h>
#include <uff/transmit_wave.h>
#include <uff/types.h>
#include <uff/uff.h>
#include <uff/wave.h>
#include <uff/writer.h>
#include <algorithm>
#include <catch2/catch.hpp>
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
  constexpr uint8_t NB_CHANNELS = 32;
  constexpr uint8_t NB_SAMPLES = 10;
  constexpr uint8_t NB_FRAMES = 1;
  constexpr uint8_t NB_PW = 1;

  auto dataset = std::make_shared<uff::Dataset<float>>();

  uff::ChannelData<float>& channelData = dataset->channelData();
  channelData.setAuthors("Unknown");
  channelData.setDescription("Missing description");
  channelData.setLocalTime("1970-01-01T00:00:00");
  channelData.setCountryCode("FR");
  channelData.setSystem("Unknown");
  channelData.setRepetitionRate(1.0);
  channelData.setSoundSpeed(1540.);

  channelData.setNumberOfFrames(NB_FRAMES);
  channelData.setNumberOfEvents(NB_PW);
  channelData.setNumberOfChannels(NB_CHANNELS);
  channelData.setNumberOfSamples(NB_SAMPLES);
  channelData.allocate();
  std::vector<float> dataVec(320);
  short maxLimit = std::numeric_limits<short>::max();
  for (auto& data : dataVec) {
    data = static_cast<short>(std::rand() / (RAND_MAX / maxLimit));
  }
  channelData.setData(dataVec);

  // Probe RCA
  // RCA made of 2 linear array
  auto probe = std::make_shared<uff::RcaArray>(channelData.numberOfChannels(),
                                               channelData.numberOfChannels());
  double pitch_x = 0.42;
  probe->setPitchX(pitch_x);
  double pitch_y = 0.42;
  probe->setPitchY(pitch_y);
  channelData.addProbe(probe);

  // Channel mapping
  std::vector<int> mapping_x(NB_CHANNELS);
  std::vector<int> mapping_y(NB_CHANNELS);
  for (int i = 0; i < NB_CHANNELS; ++i) {
    mapping_x[i] = i;
    mapping_y[i] = i + NB_CHANNELS;
  }

  // Receive setup
  uff::ReceiveSetup receiveSetup_y;
  receiveSetup_y.setProbe(probe);
  receiveSetup_y.setTimeOffset(0);
  receiveSetup_y.setSamplingFrequency(dataset->getSamplingFrequency());
  receiveSetup_y.setChannelMapping(mapping_y);

  std::vector<uff::TimedEvent> sequence;

  // Plane waves
  for (int pw = 0; pw < NB_PW; ++pw) {
    // plane wave. origin.translation is the direction vector
    auto wave = std::make_shared<uff::Wave>();
    wave->setWaveType(uff::WaveType::PLANE_WAVE);
    wave->setOrigin(uff::Transform(uff::Rotation(), uff::Translation()));

    uff::Excitation excitation;
    excitation.setTransmitFrequency(dataset->getTransmitFrequency());
    wave->setExcitation(excitation);
    channelData.addUniqueWave(wave);

    uff::TransmitWave transmitWave;
    transmitWave.setWave(wave);
    transmitWave.setTimeOffset(0.);

    uff::TransmitSetup transmitSetup;
    transmitSetup.setProbe(probe);
    transmitSetup.setTransmitWave(transmitWave);

    auto event = std::make_shared<uff::Event>();
    event->setTransmitSetup(transmitSetup);
    event->setReceiveSetup(receiveSetup_y);
    channelData.addUniqueEvent(event);

    // plane-waves have 125us delay
    uff::TimedEvent timedEvent;
    timedEvent.setEvent(event);
    timedEvent.setTimeOffset(pw * 125e-6);

    // Enqueue the plane-wave
    sequence.push_back(timedEvent);
  }

  channelData.setSequence(sequence);

  dataset->setChannelData(channelData);

  std::string filename = "tmp.uff";
  uff::Writer<float> floatWriter;
  uff::Reader<float> floatReader;

  floatWriter.setFileName(filename);
  floatWriter.setDataset(dataset);
  floatWriter.writeToFile();

  std::shared_ptr<uff::Dataset<short>> readShortDataset =
      uff::Reader<short>::loadFile(filename, true, false);
  floatReader.setFileName(filename);
  floatReader.updateMetadata();
  std::shared_ptr<uff::Dataset<float>> readFloatDataset = floatReader.dataset();

  const std::vector<short>& shortVec = readShortDataset->channelData().data();
  const std::vector<float>& floatVec = readFloatDataset->channelData().data();
  REQUIRE(std::equal(shortVec.begin(), shortVec.end(), floatVec.begin()));

  REQUIRE(readShortDataset->channelData().authors() == channelData.authors());
  REQUIRE(readShortDataset->channelData().description() == channelData.description());
  REQUIRE(readShortDataset->channelData().localTime() == channelData.localTime());
  REQUIRE(readShortDataset->channelData().countryCode() == channelData.countryCode());
  REQUIRE(readShortDataset->channelData().system() == channelData.system());
  REQUIRE(readShortDataset->channelData().repetitionRate() == channelData.repetitionRate());
  REQUIRE(readShortDataset->getSoundSpeed() == channelData.soundSpeed());
  {
    std::vector<double> geometry1 = readShortDataset->getChannelGeometry<double>();
    std::vector<double> geometry2 = channelData.channelGeometry<double>();
    REQUIRE(geometry1.size() == 4UL * 2 * NB_CHANNELS);
    REQUIRE(geometry2.size() == 4UL * 2 * NB_CHANNELS);
    REQUIRE(std::equal(geometry1.begin(), geometry1.end(), geometry2.begin()));
  }
  REQUIRE(readShortDataset->getReceiveDelay() == channelData.receiveDelay());
  REQUIRE(readShortDataset->getSamplingType() == channelData.samplingType());
  REQUIRE(std::isnan(readShortDataset->getSamplingFrequency()));
  REQUIRE(std::isnan(channelData.samplingFrequency()));
  REQUIRE(std::isnan(readShortDataset->getTransmitFrequency()));
  REQUIRE(std::isnan(channelData.transmitFrequency()));
}

}  // namespace uff::test
