/*!
 * Copyright Moduleus
 * \file uff_reader.cpp
 * \brief
 */

#ifdef WITH_HDF5

#include "uff_reader.h"

#include <limits>
#include <type_traits>

//static_assert(sizeof (long) == sizeof (long long),
//              "Architecture not supported, cf. Reader::readIntegerArrayDataset");

namespace uff {

void Reader::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
}

void Reader::updateMetadata() {
  m_dataset = std::make_shared<uff::Dataset>();

  try {
    H5::Exception::dontPrint();

    H5::H5File file(m_fileName, H5F_ACC_RDONLY);

    // Version
    H5::Group version(file.openGroup("version"));
    readVersion(version);

    // Channel Data
    H5::Group channelData(file.openGroup("channel_data"));
    readChannelData(channelData);

    file.close();
  }  // end of try block
  // catch failure caused by the H5File operations
  catch (const H5::FileIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return;
  }
  // catch failure caused by the DataSet operations
  catch (const H5::DataSetIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return;
  }
  // catch failure caused by the DataSpace operations
  catch (const H5::DataSpaceIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return;
  }
  // catch failure caused by the DataSpace operations
  catch (const H5::DataTypeIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return;
  }
  // catch failure caused by the Group operations
  catch (const H5::GroupIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return;
  }
}

uff::Aperture Reader::readAperture(const H5::Group& group) {
  uff::Aperture aperture;

  // origin
  aperture.setOrigin(readTransform(group.openGroup("origin")));

  // "window"
  aperture.setWindow(readOptionalStringDataset(group, "window"));

  // "f_number"
  aperture.setFNumber(readOptionalFloatingTypeDataset(group, "f_number"));

  // "fixed_size"
  aperture.setFixedSize(readOptionalFloatingTypeDataset(group, "fixed_size"));

  // "maximum_size" TODO
  // "minimum_size" TODO

  return aperture;
}

void Reader::readChannelData(const H5::Group& group) {
  uff::ChannelData& channelData = m_dataset->channelData();
  channelData.setAuthors(readStringDataset(group, "authors"));
  channelData.setDescription(readStringDataset(group, "description"));
  channelData.setLocalTime(readStringDataset(group, "local_time"));
  channelData.setCountryCode(readStringDataset(group, "country_code"));
  channelData.setSystem(readStringDataset(group, "system"));
  channelData.setSoundSpeed(readFloatingTypeDataset(group, "sound_speed"));
  channelData.setRepetitionRate(readOptionalFloatingTypeDataset(group, "repetition_rate"));

  // channel_data.data
  std::vector<size_t> dataDims;
  readFloatingTypeArrayDataset(group, "data", m_dataset->channelData().data(), dataDims);
  if (dataDims.size() != 4) {
    std::cerr << "Uff::Reader : Dataset dimension != 4" << std::endl;
    return;
  }
  channelData.setNumberOfFrames(static_cast<int>(dataDims[0]));
  channelData.setNumberOfEvents(static_cast<int>(dataDims[1]));
  channelData.setNumberOfChannels(static_cast<int>(dataDims[2]));
  channelData.setNumberOfSamples(static_cast<int>(dataDims[3]));

  // Probes
  H5::Group probes(group.openGroup("probes"));
  channelData.setProbes(readProbeArray(probes));

  // Unique waves
  H5::Group waves(group.openGroup("unique_waves"));
  channelData.setUniqueWaves(readWaveArray(waves));

  // Unique events
  H5::Group uniqueEvents(group.openGroup("unique_events"));
  channelData.setUniqueEvents(readEventArray(uniqueEvents));

  // Sequence
  H5::Group sequence(group.openGroup("sequence"));
  channelData.setSequence(readTimedEventArray(sequence));
}

FloatingType Reader::readFloatingTypeDataset(const H5::Group& group, const std::string& name) {
  H5::StrType datatype(H5FloatingType);
  H5::DataSet dataset = group.openDataSet(name);
  FloatingType value;
  dataset.read(&value, datatype);
  dataset.close();
  return value;
}

std::optional<FloatingType> Reader::readOptionalFloatingTypeDataset(const H5::Group& group,
                                                                    const std::string& name) {
  H5::StrType datatype(H5FloatingType);
  H5::DataSet dataset = group.openDataSet(name);
  FloatingType value;
  dataset.read(&value, datatype);
  dataset.close();
  std::optional<FloatingType> result = std::nullopt;
  if (!std::isnan(value)) {
    result = value;
  }
  return result;
}

uff::Element Reader::readElement(const H5::Group& group) {
  uff::Element element;

  element.setX(readOptionalFloatingTypeDataset(group, "x"));
  element.setY(readOptionalFloatingTypeDataset(group, "y"));
  element.setZ(readOptionalFloatingTypeDataset(group, "z"));

  return element;
}

std::vector<uff::Element> Reader::readElementArray(const H5::Group& group) {
  std::vector<uff::Element> elements;

  char buf[9];
  int id = 1;
  while (true) {
    snprintf(buf, sizeof buf, "%08d", id);
    if (H5Lexists(group.getLocId(), buf, H5P_DEFAULT)) {
      H5::Group element = group.openGroup(buf);
      elements.push_back(readElement(element));
      id++;
    } else {
      break;
    }
  }

  return elements;
}

std::shared_ptr<uff::Event> Reader::readEvent(const H5::Group& group) {
  auto event = std::make_shared<uff::Event>();

  // "receive_setup"
  event->setReceiveSetup(readReceiveSetup(group.openGroup("receive_setup")));

  // "transmit_setup"
  event->setTransmitSetup(readTransmitSetup(group.openGroup("transmit_setup")));

  return event;
}

std::vector<std::shared_ptr<uff::Event>> Reader::readEventArray(const H5::Group& group) {
  std::vector<std::shared_ptr<uff::Event>> events;

  char buf[9];
  int id = 1;
  while (true) {
    snprintf(buf, sizeof buf, "%08d", id);

    if (H5Lexists(group.getLocId(), buf, H5P_DEFAULT)) {
      H5::Group event = group.openGroup(buf);
      events.push_back(readEvent(event));
      id++;
    } else {
      break;
    }
  }

  return events;
}

uff::Excitation Reader::readExcitation(const H5::Group& group) {
  uff::Excitation excitation;

  // "pulse_shape"
  excitation.setPulseShape(readOptionalStringDataset(group, "pulse_shape"));

  // "transmit_frequency"
  excitation.setTransmitFrequency(readOptionalFloatingTypeDataset(group, "transmit_frequency"));

  // "waveform"
  // TODO excitation.setWaveform(readFloatingTypeArrayDataset(group, "waveform"));

  // "sampling_frequency"
  excitation.setSamplingFrequency(readOptionalFloatingTypeDataset(group, "sampling_frequency"));

  return excitation;
}

void Reader::readFloatingTypeArrayDataset(const H5::Group& group, const std::string& name,
                                          std::vector<FloatingType>& values,
                                          std::vector<size_t>& dimensions) {
  H5::DataSet dataset = group.openDataSet(name);
  // TODO: check if type is correct : dataset.getTypeClass()
  H5::StrType datatype(H5FloatingType);

  // find dataset dimensions
  H5::DataSpace dataspace = dataset.getSpace();
  int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims((unsigned long long*)dimensions.data());  // Poor casting
  size_t numel = 1;
  for (auto sz : dimensions) {
    numel *= sz;
    //std::cout << "sz:" << sz << std::endl;
  }

  // reserve space in the output buffer
  values.resize(numel);

  // read data
  dataset.read(values.data(), datatype);
}

void Reader::readIntegerArrayDataset(const H5::Group& group, const std::string& name,
                                     std::vector<int>& values, std::vector<size_t>& dimensions) {
  H5::DataSet dataset = group.openDataSet(name);
  // TODO: check if type is correct : dataset.getTypeClass()
  H5::StrType datatype(H5::PredType::NATIVE_INT);

  // find dataset dimensions
  H5::DataSpace dataspace = dataset.getSpace();
  int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(
      (hsize_t*)dimensions
          .data());  // NOTE: Will bug on architectures whose (sizeof(hsize_t != sizeof(long long unsigned int))) cf. the static_assert upper
  //hsize_t* dims = dimensions.data();
  //dataspace.getSimpleExtentDims(dims);
  size_t numel = 1;
  for (auto sz : dimensions) {
    numel *= sz;
    //std::cout << "sz:" << sz << std::endl;
  }

  // reserve space in the output buffer
  values.resize(numel);

  // read data
  dataset.read(values.data(), datatype);
}

int Reader::readIntegerDataset(const H5::Group& group, const std::string& name) {
  H5::StrType datatype(H5::PredType::NATIVE_INT);
  H5::DataSet dataset = group.openDataSet(name);
  int value;
  dataset.read(&value, datatype);
  dataset.close();
  return value;
}

std::shared_ptr<uff::LinearArray> Reader::readLinearArray(const H5::Group& group) {
  auto linearArray =
      std::make_shared<uff::LinearArray>(readIntegerDataset(group, "number_elements"));

  // Read "pitch"
  linearArray->setPitch(readOptionalFloatingTypeDataset(group, "pitch"));

  // Read "element_width"
  linearArray->setElementWidth(readOptionalFloatingTypeDataset(group, "element_width"));

  // Read "element_height"
  linearArray->setElementHeight(readOptionalFloatingTypeDataset(group, "element_height"));

  return linearArray;
}

std::shared_ptr<uff::MatrixArray> Reader::readMatrixArray(const H5::Group& group) {
  auto matrixArray = std::make_shared<uff::MatrixArray>();

  // Read "number_elements"
  matrixArray->setNumberElementsX(readIntegerDataset(group, "number_elements_x"));
  matrixArray->setNumberElementsY(readIntegerDataset(group, "number_elements_y"));

  // Read "pitch"
  matrixArray->setPitchX(readOptionalFloatingTypeDataset(group, "pitch_x"));
  matrixArray->setPitchY(readOptionalFloatingTypeDataset(group, "pitch_y"));

  // Read "element_width"
  matrixArray->setElementWidth(readOptionalFloatingTypeDataset(group, "element_width"));

  // Read "element_height"
  matrixArray->setElementHeight(readOptionalFloatingTypeDataset(group, "element_height"));

  return matrixArray;
}

std::shared_ptr<RcaArray> Reader::readRcaArray(const H5::Group& group) {
  auto rcaArray = std::make_shared<uff::RcaArray>(readIntegerDataset(group, "number_elements_x"),
                                                  readIntegerDataset(group, "number_elements_y"));

  // Read "pitch"
  rcaArray->setPitchX(readOptionalFloatingTypeDataset(group, "pitch_x"));
  rcaArray->setPitchY(readOptionalFloatingTypeDataset(group, "pitch_y"));

  // Read "element_width"
  rcaArray->setElementWidthX(readOptionalFloatingTypeDataset(group, "element_width_x"));
  rcaArray->setElementWidthY(readOptionalFloatingTypeDataset(group, "element_width_y"));

  // Read "element_height"
  rcaArray->setElementHeightX(readOptionalFloatingTypeDataset(group, "element_height_x"));
  rcaArray->setElementHeightY(readOptionalFloatingTypeDataset(group, "element_height_y"));

  return rcaArray;
}

std::shared_ptr<uff::Probe> Reader::readProbe(const H5::Group& group) {
  auto probe = std::make_shared<uff::Probe>();

  // transform
  H5::Group transform = group.openGroup("transform");
  probe->setTransform(readTransform(transform));

  // focal length
  probe->setFocalLength(readOptionalFloatingTypeDataset(group, "focal_length"));

  // elements
  H5::Group elements = group.openGroup("elements");
  probe->setElements(readElementArray(elements));

  // element geometries (TODO)

  // impulse responses (TODO)

  // probe_type (optional)
  if (H5Lexists(group.getLocId(), "probe_type", H5P_DEFAULT)) {
    std::string probeType = readStringDataset(group, "probe_type");
    if (probeType == "LinearArray") {
      auto linearArray = readLinearArray(group);
      linearArray->setTransform(probe->transform());
      linearArray->setFocalLength(probe->focalLength());
      linearArray->setElements(probe->elements());
      linearArray->setElementGeometries(probe->elementGeometries());
      linearArray->setImpulseResponses(probe->impulseResponses());
      return linearArray;
    } else if (probeType == "MatrixArray") {
      auto matrixArray = readMatrixArray(group);
      matrixArray->setTransform(probe->transform());
      matrixArray->setFocalLength(probe->focalLength());
      matrixArray->setElements(probe->elements());
      matrixArray->setElementGeometries(probe->elementGeometries());
      matrixArray->setImpulseResponses(probe->impulseResponses());
      return matrixArray;
    } else if (probeType == "RcaArray") {
      auto rcaArray = readRcaArray(group);
      rcaArray->setTransform(probe->transform());
      rcaArray->setFocalLength(probe->focalLength());
      rcaArray->setElements(probe->elements());
      rcaArray->setElementGeometries(probe->elementGeometries());
      rcaArray->setImpulseResponses(probe->impulseResponses());
      return rcaArray;
    } else {
      std::cerr << getClassNameInternal() << ": Ignoring unknown probe_type:" << probeType
                << std::endl;
      return probe;
    }
  }

  return probe;
}

std::vector<std::shared_ptr<uff::Probe>> Reader::readProbeArray(const H5::Group& group) {
  std::vector<std::shared_ptr<uff::Probe>> probes;

  char buf[9];
  int id = 1;
  while (true) {
    snprintf(buf, sizeof buf, "%08d", id);
    if (H5Lexists(group.getLocId(), buf, H5P_DEFAULT)) {
      H5::Group probe = group.openGroup(buf);
      probes.push_back(readProbe(probe));
      id++;
    } else {
      break;
    }
  }

  return probes;
}

uff::ReceiveSetup Reader::readReceiveSetup(const H5::Group& group) {
  uff::ReceiveSetup receiveSetup;

  // "probe"
  int probeId = std::stoi(readStringDataset(group, "probe_id"));
  receiveSetup.setProbe(m_dataset->channelData().probes()[(size_t)probeId - 1]);

  // "time_offset"
  receiveSetup.setTimeOffset(readFloatingTypeDataset(group, "time_offset"));

  // "sampling_frequency"
  receiveSetup.setSamplingFrequency(readOptionalFloatingTypeDataset(group, "sampling_frequency"));

  // "sampling_type"
  const uff::ReceiveSetup::SAMPLING_TYPE st =
      static_cast<uff::ReceiveSetup::SAMPLING_TYPE>(readIntegerDataset(group, "sampling_type"));
  switch (st) {
    case uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF:
      receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF);
      break;
    case uff::ReceiveSetup::SAMPLING_TYPE::IQ:
      receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::IQ);
      break;
    case uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_4X_F0:
      receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_4X_F0);
      break;
    case uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_2X_F0:
      receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_2X_F0);
      break;
    default:
      std::cerr << "Unknow sampling type:" << static_cast<int>(st);
  }

  // channel_mapping [optional]
  if (H5Lexists(group.getLocId(), "channel_mapping", H5P_DEFAULT)) {
    std::vector<int> channelMapping;
    std::vector<size_t> dimensions;
    readIntegerArrayDataset(group, "channel_mapping", channelMapping, dimensions);
    receiveSetup.setChannelMapping(channelMapping);
  }

  // "tgc_profile" [optional]
  if (H5Lexists(group.getLocId(), "tgc_profile", H5P_DEFAULT)) {
    std::vector<FloatingType> tgcProfile;
    std::vector<size_t> dimensions;
    readFloatingTypeArrayDataset(group, "tgc_profile", tgcProfile, dimensions);
    receiveSetup.setTgcProfile(tgcProfile);
  }

  // "tgc_sampling_frequency" [optional]
  if (H5Lexists(group.getLocId(), "tgc_sampling_frequency", H5P_DEFAULT)) {
    receiveSetup.setTgcSamplingFrequency(
        readOptionalFloatingTypeDataset(group, "tgc_sampling_frequency"));
  }

  // "modulation_frequency" [optional]
  if (H5Lexists(group.getLocId(), "tgc_sampling_frequency", H5P_DEFAULT)) {
    receiveSetup.setModulationFrequency(
        readOptionalFloatingTypeDataset(group, "modulation_frequency"));
  }

  return receiveSetup;
}

uff::Rotation Reader::readRotation(const H5::Group& group) {
  uff::Rotation rotation;

  rotation.setX(readFloatingTypeDataset(group, "x"));
  rotation.setY(readFloatingTypeDataset(group, "y"));
  rotation.setZ(readFloatingTypeDataset(group, "z"));

  return rotation;
}

std::string Reader::readStringDataset(const H5::Group& group, const std::string& name) {
  H5::StrType datatype(0, H5T_VARIABLE);
  H5::DataSpace dataspace(H5S_SCALAR);
  H5::DataSet dataset = group.openDataSet(name);
  std::string buffer;
  dataset.read(buffer, datatype, dataspace);
  return buffer;
}

std::optional<std::string> Reader::readOptionalStringDataset(const H5::Group& group,
                                                             const std::string& name) {
  H5::StrType datatype(0, H5T_VARIABLE);
  H5::DataSpace dataspace(H5S_SCALAR);
  H5::DataSet dataset = group.openDataSet(name);
  std::string buffer;
  dataset.read(buffer, datatype, dataspace);
  std::optional<std::string> result = std::nullopt;
  if (buffer != "undefined") result = buffer;
  return result;
}

uff::TimedEvent Reader::readTimedEvent(const H5::Group& group) {
  uff::TimedEvent timedEvent;

  // "event"
  int eventId = std::stoi(readStringDataset(group, "event_id"));
  //std::cout << "probeId" << probeId << " " << m_dataset.channelData().probes().size();
  timedEvent.setEvent(m_dataset->channelData().uniqueEvents()[(size_t)eventId - 1]);

  // "time_offset"
  timedEvent.setTimeOffset(readFloatingTypeDataset(group, "time_offset"));

  return timedEvent;
}

std::vector<uff::TimedEvent> Reader::readTimedEventArray(const H5::Group& group) {
  std::vector<uff::TimedEvent> timedEvents;

  char buf[9];
  int id = 1;
  while (true) {
    snprintf(buf, sizeof buf, "%08d", id);

    if (H5Lexists(group.getLocId(), buf, H5P_DEFAULT)) {
      H5::Group timedEvent = group.openGroup(buf);
      timedEvents.push_back(readTimedEvent(timedEvent));
      id++;
    } else {
      break;
    }
  }

  return timedEvents;
}

uff::Transform Reader::readTransform(const H5::Group& group) {
  // rotation
  uff::Rotation rotation = readRotation(group.openGroup("rotation"));

  // translation
  uff::Translation translation = readTranslation(group.openGroup("translation"));

  return uff::Transform(rotation, translation);
}

uff::Translation Reader::readTranslation(const H5::Group& group) {
  uff::Translation translation;

  translation.setX(readFloatingTypeDataset(group, "x"));
  translation.setY(readFloatingTypeDataset(group, "y"));
  translation.setZ(readFloatingTypeDataset(group, "z"));

  return translation;
}

uff::TransmitSetup Reader::readTransmitSetup(const H5::Group& group) {
  uff::TransmitSetup transmitSetup;

  // "probe"
  int probeId = std::stoi(readStringDataset(group, "probe_id"));
  transmitSetup.setProbe(m_dataset->channelData().probes()[(size_t)probeId - 1]);

  // "transmit_wave"
  transmitSetup.setTransmitWave(readTransmitWave(group.openGroup("transmit_wave")));

  // channel_mapping [optional]
  if (H5Lexists(group.getLocId(), "channel_mapping", H5P_DEFAULT)) {
    std::vector<int> channelMapping;
    std::vector<size_t> dimensions;
    readIntegerArrayDataset(group, "channel_mapping", channelMapping, dimensions);
    transmitSetup.setChannelMapping(channelMapping);
  }

  return transmitSetup;
}

uff::TransmitWave Reader::readTransmitWave(const H5::Group& group) {
  uff::TransmitWave transmitWave;

  // "wave"
  int waveId = std::stoi(readStringDataset(group, "wave_id"));
  transmitWave.setWave(m_dataset->channelData().uniqueWaves()[(size_t)waveId - 1]);

  // "time_offset"
  transmitWave.setTimeOffset(readFloatingTypeDataset(group, "time_offset"));

  // "weight"
  transmitWave.setWeight(readFloatingTypeDataset(group, "weight"));

  return transmitWave;
}

void Reader::readVersion(const H5::Group& group) {
  int major = readIntegerDataset(group, "major");
  int minor = readIntegerDataset(group, "minor");
  int patch = readIntegerDataset(group, "patch");
  uff::Version version(major, minor, patch);
  m_dataset->setVersion(version);
}

std::shared_ptr<uff::Wave> Reader::readWave(const H5::Group& group) {
  auto wave = std::make_shared<uff::Wave>();

  // write "origin"
  wave->setOrigin(readTransform(group.openGroup("origin")));

  // write "wave_type"
  const uff::WaveType wt = static_cast<uff::WaveType>(readIntegerDataset(group, "wave_type"));
  switch (wt) {
    case uff::WaveType::CONVERGING_WAVE:
      wave->setWaveType(uff::WaveType::CONVERGING_WAVE);
      break;
    case uff::WaveType::CYLINDRICAL_WAVE:
      wave->setWaveType(uff::WaveType::CYLINDRICAL_WAVE);
      break;
    case uff::WaveType::DIVERGING_WAVE:
      wave->setWaveType(uff::WaveType::DIVERGING_WAVE);
      break;
    case uff::WaveType::PLANE_WAVE:
      wave->setWaveType(uff::WaveType::PLANE_WAVE);
      break;
    default:
      // TODO: unknown wave type ?!?
      break;
  }

  // write "aperture"
  wave->setAperture(readAperture(group.openGroup("aperture")));

  // write "excitation"
  wave->setExcitation(readExcitation(group.openGroup("excitation")));

  return wave;
}

std::vector<std::shared_ptr<uff::Wave>> Reader::readWaveArray(const H5::Group& group) {
  std::vector<std::shared_ptr<uff::Wave>> waves;

  char buf[9];
  int id = 1;
  while (true) {
    snprintf(buf, sizeof buf, "%08d", id);
    if (H5Lexists(group.getLocId(), buf, H5P_DEFAULT)) {
      H5::Group wave = group.openGroup(buf);
      waves.push_back(readWave(wave));
      id++;
    } else {
      break;
    }
  }

  return waves;
}

}  // namespace uff
#endif  // WITH_HDF5
