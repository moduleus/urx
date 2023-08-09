#include <catch2/catch.hpp>

namespace uff::test {

TEST_CASE("Load HDF5 file", "[hdf5_loader]") {
  // constexpr uint8_t NB_CHANNELS = 32;
  // constexpr uint8_t NB_SAMPLES = 10;
  // constexpr uint8_t NB_FRAMES = 1;
  // constexpr uint8_t NB_PW = 1;

  // auto dataset = std::make_shared<uff::Dataset>();

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
  // auto probe = std::make_shared<uff::RcaArray>(
  //     Point2D<uint32_t>{channelData.numberOfChannels(), channelData.numberOfChannels()});
  // Point2D<double> pitch{0.42, 0.42};
  // probe->setPitch(pitch);
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

  // std::vector<uff::TimedEvent> sequence;

  // // Plane waves
  // for (int pw = 0; pw < NB_PW; ++pw) {
  //   // plane wave. origin.translation is the direction vector
  //   auto wave = std::make_shared<uff::Wave>();
  //   wave->setWaveType(uff::WaveType::PLANE_WAVE);
  //   wave->setOrigin(uff::Transform(uff::Point3D<MetadataType>(), uff::Point3D<MetadataType>()));

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
  //   uff::TimedEvent timedEvent;
  //   timedEvent.setEvent(event);
  //   timedEvent.setTimeOffset(pw * 125e-6);

  //   // Enqueue the plane-wave
  //   sequence.push_back(timedEvent);
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
  // for (size_t i = 0; i < floatVec.size(); ++i) {
  //   REQUIRE(floatVec[i] == static_cast<float>(shortVec[i]));
  // }
}

}  // namespace uff::test
