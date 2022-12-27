/*!
 * Copyright Moduleus
 * \file uff/reader.cpp
 * \brief
 */

#ifdef WITH_HDF5

#include "uff/reader.h"
#include "uff/log.h"

#include <limits>
#include <type_traits>

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif

//static_assert(sizeof (long) == sizeof (long long),
//              "Architecture not supported, cf. Reader<DataType>::readIntegerArrayDataset");

namespace uff {

template <typename DataType>
void Reader<DataType>::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
}

template <typename DataType>
void Reader<DataType>::updateMetadata() {
  m_dataset = std::make_shared<uff::Dataset<DataType>>();
  m_dataset->channelData().setSkipChannelDataData(m_skipChannelDataData);

  H5::Exception::dontPrint();

  H5::H5File file(m_fileName, H5F_ACC_RDONLY);

  // Version
  H5::Group version(file.openGroup("version"));
  readVersion(version);

  // Channel Data
  H5::Group channelData(file.openGroup("channel_data"));
  readChannelData(channelData);

  file.close();
}

template <typename DataType>
uff::Aperture Reader<DataType>::readAperture(const H5::Group& group) {
  uff::Aperture aperture;

  // origin
  aperture.setOrigin(readTransform(group.openGroup("origin")));

  // "window"
  aperture.setWindow(readOptionalStringDataset(group, "window"));

  // "f_number"
  aperture.setFNumber(readOptionalMetadataTypeDataset(group, "f_number"));

  // "fixed_size"
  aperture.setFixedSize(readOptionalMetadataTypeDataset(group, "fixed_size"));

  // "maximum_size" TODO
  // "minimum_size" TODO

  return aperture;
}

template <typename DataType>
void Reader<DataType>::readChannelData(const H5::Group& group) {
  uff::ChannelData<DataType>& channelData = m_dataset->channelData();
  channelData.setAuthors(readStringDataset(group, "authors"));
  channelData.setDescription(readStringDataset(group, "description"));
  channelData.setLocalTime(readStringDataset(group, "local_time"));
  channelData.setCountryCode(readStringDataset(group, "country_code"));
  channelData.setSystem(readStringDataset(group, "system"));
  channelData.setSoundSpeed(readMetadataTypeDataset(group, "sound_speed"));
  channelData.setRepetitionRate(readOptionalMetadataTypeDataset(group, "repetition_rate"));

  if (!m_skipChannelDataData && group.openDataSet("data").getDataType() != H5DataType) {
    LOG_THIS(ERROR) << "Invalid format of data.\n";
    return;
  }
  // channel_data.data
  std::vector<size_t> dataDims;
  std::vector<DataType> fake;
  readDataTypeArrayDataset(
      group, "data", m_skipChannelDataData ? fake : m_dataset->channelData().data(), dataDims);
  if (dataDims.size() != 4) {
    LOG_THIS(ERROR) << "Dataset dimension != 4\n";
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

template <typename DataType>
MetadataType Reader<DataType>::readMetadataTypeDataset(const H5::Group& group,
                                                       const std::string& name) {
  H5::StrType datatype(H5MetadataType);
  H5::DataSet dataset = group.openDataSet(name);
  MetadataType value;
  dataset.read(&value, datatype);
  dataset.close();
  return value;
}

template <typename DataType>
std::optional<MetadataType> Reader<DataType>::readOptionalMetadataTypeDataset(
    const H5::Group& group, const std::string& name) {
  H5::StrType datatype(H5MetadataType);
  H5::DataSet dataset = group.openDataSet(name);
  MetadataType value;
  dataset.read(&value, datatype);
  dataset.close();
  std::optional<MetadataType> result = std::nullopt;
  if (!std::isnan(value)) {
    result = value;
  }
  return result;
}

template <typename DataType>
uff::Element Reader<DataType>::readElement(const H5::Group& group) {
  uff::Element element;

  element.setX(readOptionalMetadataTypeDataset(group, "x"));
  element.setY(readOptionalMetadataTypeDataset(group, "y"));
  element.setZ(readOptionalMetadataTypeDataset(group, "z"));

  return element;
}

template <typename DataType>
std::vector<uff::Element> Reader<DataType>::readElementArray(const H5::Group& group) {
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

template <typename DataType>
std::shared_ptr<uff::Event> Reader<DataType>::readEvent(const H5::Group& group) {
  auto event = std::make_shared<uff::Event>();

  // "receive_setup"
  event->setReceiveSetup(readReceiveSetup(group.openGroup("receive_setup")));

  // "transmit_setup"
  event->setTransmitSetup(readTransmitSetup(group.openGroup("transmit_setup")));

  return event;
}

template <typename DataType>
std::vector<std::shared_ptr<uff::Event>> Reader<DataType>::readEventArray(const H5::Group& group) {
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

template <typename DataType>
uff::Excitation Reader<DataType>::readExcitation(const H5::Group& group) {
  uff::Excitation excitation;

  // "pulse_shape"
  excitation.setPulseShape(readOptionalStringDataset(group, "pulse_shape"));

  // "transmit_frequency"
  excitation.setTransmitFrequency(readOptionalMetadataTypeDataset(group, "transmit_frequency"));

  // "waveform"
  // TODO excitation.setWaveform(readMetadataTypeArrayDataset(group, "waveform"));

  // "sampling_frequency"
  excitation.setSamplingFrequency(readOptionalMetadataTypeDataset(group, "sampling_frequency"));

  return excitation;
}

template <typename DataType>
void Reader<DataType>::readDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                                std::vector<DataType>& values,
                                                std::vector<size_t>& dimensions) {
  H5::DataSet dataset = group.openDataSet(name);
  H5::StrType datatype(H5DataType);

  // find dataset dimensions
  H5::DataSpace dataspace = dataset.getSpace();
  int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(
      reinterpret_cast<unsigned long long*>(dimensions.data()));  // Poor casting
  size_t numel = 1;
  for (auto sz : dimensions) {
    numel *= sz;
    //std::cout << "sz:" << sz << std::endl;
  }

  if (!m_skipChannelDataData) {
    // reserve space in the output buffer
    values.resize(numel);

    // read data
    dataset.read(values.data(), datatype);
  }
}

template <typename DataType>
void Reader<DataType>::readMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                                    std::vector<MetadataType>& values,
                                                    std::vector<size_t>& dimensions) {
  H5::DataSet dataset = group.openDataSet(name);
  // TODO: check if type is correct : dataset.getTypeClass()
  H5::StrType datatype(H5MetadataType);

  // find dataset dimensions
  H5::DataSpace dataspace = dataset.getSpace();
  int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(
      reinterpret_cast<unsigned long long*>(dimensions.data()));  // Poor casting
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

template <typename DataType>
void Reader<DataType>::readIntegerArrayDataset(const H5::Group& group, const std::string& name,
                                               std::vector<int>& values,
                                               std::vector<size_t>& dimensions) {
  H5::DataSet dataset = group.openDataSet(name);
  // TODO: check if type is correct : dataset.getTypeClass()
  H5::StrType datatype(H5::PredType::NATIVE_INT);

  // find dataset dimensions
  H5::DataSpace dataspace = dataset.getSpace();
  int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(reinterpret_cast<hsize_t*>(
      dimensions
          .data()));  // NOTE: Will bug on architectures whose (sizeof(hsize_t != sizeof(long long unsigned int))) cf. the static_assert upper
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

template <typename DataType>
int Reader<DataType>::readIntegerDataset(const H5::Group& group, const std::string& name) {
  H5::StrType datatype(H5::PredType::NATIVE_INT);
  H5::DataSet dataset = group.openDataSet(name);
  int value;
  dataset.read(&value, datatype);
  dataset.close();
  return value;
}

template <typename DataType>
std::shared_ptr<uff::LinearArray> Reader<DataType>::readLinearArray(const H5::Group& group) {
  auto linearArray =
      std::make_shared<uff::LinearArray>(readIntegerDataset(group, "number_elements"));

  // Read "pitch"
  linearArray->setPitch(readOptionalMetadataTypeDataset(group, "pitch"));

  // Read "element_width"
  linearArray->setElementWidth(readOptionalMetadataTypeDataset(group, "element_width"));

  // Read "element_height"
  linearArray->setElementHeight(readOptionalMetadataTypeDataset(group, "element_height"));

  return linearArray;
}

template <typename DataType>
std::shared_ptr<uff::MatrixArray> Reader<DataType>::readMatrixArray(const H5::Group& group) {
  auto matrixArray = std::make_shared<uff::MatrixArray>();

  // Read "number_elements"
  matrixArray->setNumberElementsX(readIntegerDataset(group, "number_elements_x"));
  matrixArray->setNumberElementsY(readIntegerDataset(group, "number_elements_y"));

  // Read "pitch"
  matrixArray->setPitchX(readOptionalMetadataTypeDataset(group, "pitch_x"));
  matrixArray->setPitchY(readOptionalMetadataTypeDataset(group, "pitch_y"));

  // Read "element_width"
  matrixArray->setElementWidth(readOptionalMetadataTypeDataset(group, "element_width"));

  // Read "element_height"
  matrixArray->setElementHeight(readOptionalMetadataTypeDataset(group, "element_height"));

  return matrixArray;
}

template <typename DataType>
std::shared_ptr<RcaArray> Reader<DataType>::readRcaArray(const H5::Group& group) {
  auto rcaArray = std::make_shared<uff::RcaArray>(readIntegerDataset(group, "number_elements_x"),
                                                  readIntegerDataset(group, "number_elements_y"));

  // Read "pitch"
  rcaArray->setPitchX(readOptionalMetadataTypeDataset(group, "pitch_x"));
  rcaArray->setPitchY(readOptionalMetadataTypeDataset(group, "pitch_y"));

  // Read "element_width"
  rcaArray->setElementWidthX(readOptionalMetadataTypeDataset(group, "element_width_x"));
  rcaArray->setElementWidthY(readOptionalMetadataTypeDataset(group, "element_width_y"));

  // Read "element_height"
  rcaArray->setElementHeightX(readOptionalMetadataTypeDataset(group, "element_height_x"));
  rcaArray->setElementHeightY(readOptionalMetadataTypeDataset(group, "element_height_y"));

  return rcaArray;
}

template <typename DataType>
std::shared_ptr<uff::Probe> Reader<DataType>::readProbe(const H5::Group& group) {
  auto probe = std::make_shared<uff::Probe>();

  // transform
  H5::Group transform = group.openGroup("transform");
  probe->setTransform(readTransform(transform));

  // focal length
  probe->setFocalLength(readOptionalMetadataTypeDataset(group, "focal_length"));

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
    }
    if (probeType == "MatrixArray") {
      auto matrixArray = readMatrixArray(group);
      matrixArray->setTransform(probe->transform());
      matrixArray->setFocalLength(probe->focalLength());
      matrixArray->setElements(probe->elements());
      matrixArray->setElementGeometries(probe->elementGeometries());
      matrixArray->setImpulseResponses(probe->impulseResponses());
      return matrixArray;
    }
    if (probeType == "RcaArray") {
      auto rcaArray = readRcaArray(group);
      rcaArray->setTransform(probe->transform());
      rcaArray->setFocalLength(probe->focalLength());
      rcaArray->setElements(probe->elements());
      rcaArray->setElementGeometries(probe->elementGeometries());
      rcaArray->setImpulseResponses(probe->impulseResponses());
      return rcaArray;
    }
    LOG_THIS(ERROR) << getClassNameInternal() << ": Ignoring unknown probe_type:" << probeType
                    << "\n";
    return probe;
  }

  return probe;
}

template <typename DataType>
std::vector<std::shared_ptr<uff::Probe>> Reader<DataType>::readProbeArray(const H5::Group& group) {
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

template <typename DataType>
uff::ReceiveSetup Reader<DataType>::readReceiveSetup(const H5::Group& group) {
  uff::ReceiveSetup receiveSetup;

  // "probe"
  int probeId = std::stoi(readStringDataset(group, "probe_id"));
  receiveSetup.setProbe(m_dataset->channelData().probes()[static_cast<size_t>(probeId) - 1]);

  // "time_offset"
  receiveSetup.setTimeOffset(readMetadataTypeDataset(group, "time_offset"));

  // "sampling_frequency"
  receiveSetup.setSamplingFrequency(readOptionalMetadataTypeDataset(group, "sampling_frequency"));

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
      LOG_THIS(ERROR) << "Unknow sampling type:" << static_cast<int>(st);
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
    std::vector<MetadataType> tgcProfile;
    std::vector<size_t> dimensions;
    readMetadataTypeArrayDataset(group, "tgc_profile", tgcProfile, dimensions);
    receiveSetup.setTgcProfile(tgcProfile);
  }

  // "tgc_sampling_frequency" [optional]
  if (H5Lexists(group.getLocId(), "tgc_sampling_frequency", H5P_DEFAULT)) {
    receiveSetup.setTgcSamplingFrequency(
        readOptionalMetadataTypeDataset(group, "tgc_sampling_frequency"));
  }

  // "modulation_frequency" [optional]
  if (H5Lexists(group.getLocId(), "tgc_sampling_frequency", H5P_DEFAULT)) {
    receiveSetup.setModulationFrequency(
        readOptionalMetadataTypeDataset(group, "modulation_frequency"));
  }

  return receiveSetup;
}

template <typename DataType>
uff::Rotation Reader<DataType>::readRotation(const H5::Group& group) {
  uff::Rotation rotation;

  rotation.setX(readMetadataTypeDataset(group, "x"));
  rotation.setY(readMetadataTypeDataset(group, "y"));
  rotation.setZ(readMetadataTypeDataset(group, "z"));

  return rotation;
}

template <typename DataType>
std::string Reader<DataType>::readStringDataset(const H5::Group& group, const std::string& name) {
  H5::StrType datatype(0, H5T_VARIABLE);
  H5::DataSpace dataspace(H5S_SCALAR);
  H5::DataSet dataset = group.openDataSet(name);
  std::string buffer;
  dataset.read(buffer, datatype, dataspace);
  return buffer;
}

template <typename DataType>
std::optional<std::string> Reader<DataType>::readOptionalStringDataset(const H5::Group& group,
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

template <typename DataType>
uff::TimedEvent Reader<DataType>::readTimedEvent(const H5::Group& group) {
  uff::TimedEvent timedEvent;

  // "event"
  int eventId = std::stoi(readStringDataset(group, "event_id"));
  //std::cout << "probeId" << probeId << " " << m_dataset.channelData().probes().size();
  timedEvent.setEvent(m_dataset->channelData().uniqueEvents()[static_cast<size_t>(eventId) - 1]);

  // "time_offset"
  timedEvent.setTimeOffset(readMetadataTypeDataset(group, "time_offset"));

  return timedEvent;
}

template <typename DataType>
std::vector<uff::TimedEvent> Reader<DataType>::readTimedEventArray(const H5::Group& group) {
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

template <typename DataType>
uff::Transform Reader<DataType>::readTransform(const H5::Group& group) {
  // rotation
  uff::Rotation rotation = readRotation(group.openGroup("rotation"));

  // translation
  uff::Translation translation = readTranslation(group.openGroup("translation"));

  return uff::Transform(rotation, translation);
}

template <typename DataType>
uff::Translation Reader<DataType>::readTranslation(const H5::Group& group) {
  uff::Translation translation;

  translation.setX(readMetadataTypeDataset(group, "x"));
  translation.setY(readMetadataTypeDataset(group, "y"));
  translation.setZ(readMetadataTypeDataset(group, "z"));

  return translation;
}

template <typename DataType>
uff::TransmitSetup Reader<DataType>::readTransmitSetup(const H5::Group& group) {
  uff::TransmitSetup transmitSetup;

  // "probe"
  int probeId = std::stoi(readStringDataset(group, "probe_id"));
  transmitSetup.setProbe(m_dataset->channelData().probes()[static_cast<size_t>(probeId) - 1]);

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

template <typename DataType>
uff::TransmitWave Reader<DataType>::readTransmitWave(const H5::Group& group) {
  uff::TransmitWave transmitWave;

  // "wave"
  int waveId = std::stoi(readStringDataset(group, "wave_id"));
  transmitWave.setWave(m_dataset->channelData().uniqueWaves()[static_cast<size_t>(waveId) - 1]);

  // "time_offset"
  transmitWave.setTimeOffset(readMetadataTypeDataset(group, "time_offset"));

  // "weight"
  transmitWave.setWeight(readMetadataTypeDataset(group, "weight"));

  return transmitWave;
}

template <typename DataType>
void Reader<DataType>::readVersion(const H5::Group& group) {
  int major = readIntegerDataset(group, "major");
  int minor = readIntegerDataset(group, "minor");
  int patch = readIntegerDataset(group, "patch");
  uff::Version version(major, minor, patch);
  m_dataset->setVersion(version);
}

template <typename DataType>
std::shared_ptr<uff::Wave> Reader<DataType>::readWave(const H5::Group& group) {
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

template <typename DataType>
std::vector<std::shared_ptr<uff::Wave>> Reader<DataType>::readWaveArray(const H5::Group& group) {
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

template class Reader<float>;
template class Reader<short>;

}  // namespace uff
#endif  // WITH_HDF5
