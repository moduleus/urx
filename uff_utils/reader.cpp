#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <utility>

#include <uff/aperture.h>
#include <uff/channel_data.h>
#include <uff/dataset.h>
#include <uff/event.h>
#include <uff/excitation.h>
#include <uff/linear_array.h>
#include <uff/matrix_array.h>
#include <uff/probe.h>
#include <uff/rca_array.h>
#include <uff/receive_setup.h>
#include <uff/timed_event.h>
#include <uff/transform.h>
#include <uff/transmit_setup.h>
#include <uff/transmit_wave.h>
#include <uff/types.h>
#include <uff/uff.h>
#include <uff/version.h>
#include <uff/wave.h>

#include <uff_utils/reader.h>

namespace uff {

template <typename DataType>
void Reader<DataType>::updateMetadata() {
  _dataset = std::make_shared<uff::Dataset<DataType>>();
  _dataset->channelData().setSkipChannelDataData(_skip_data);

  H5::Exception::dontPrint();

  H5::H5File file(_filename, H5F_ACC_RDONLY);

  // Version
  H5::Group version(file.openGroup("version"));
  _dataset->setVersion(readVersion(version));

  // Channel Data
  H5::Group channelData(file.openGroup("channel_data"));
  _dataset->setChannelData(readChannelData(channelData, false, _skip_data));
}

template <typename DataType>
std::shared_ptr<uff::Dataset<DataType>> Reader<DataType>::loadFile(std::string_view filename,
                                                                   bool castData,
                                                                   bool skipChannelData) {
  auto dataset = std::make_shared<uff::Dataset<DataType>>();
  dataset->channelData().setSkipChannelDataData(skipChannelData);

  H5::Exception::dontPrint();

  H5::H5File file(filename.data(), H5F_ACC_RDONLY);

  // Version
  H5::Group version(file.openGroup("version"));
  dataset->setVersion(readVersion(version));

  // Channel Data
  H5::Group channelData(file.openGroup("channel_data"));
  dataset->setChannelData(readChannelData(channelData, castData, skipChannelData));

  return dataset;
}

template <typename DataType>
uff::Aperture Reader<DataType>::readAperture(const H5::Group& group) {
  uff::Aperture aperture;

  // origin
  aperture.setOrigin(readTransform(group.openGroup("origin")));

  // "window"
  aperture.setWindow(readStringDataset(group, "window"));

  // "f_number"
  aperture.setFNumber(readOptionalMetadataTypeDataset(group, "f_number"));

  // "fixed_size"
  aperture.setFixedSize(readOptionalMetadataTypeDataset(group, "fixed_size"));

  // "maximum_size" TODO
  // "minimum_size" TODO

  return aperture;
}

template <typename DataType>
uff::ChannelData<DataType> Reader<DataType>::readChannelData(const H5::Group& group, bool castData,
                                                             bool skipData) {
  uff::ChannelData<DataType> channelData;
  channelData.setAuthors(readStringDataset(group, "authors"));
  channelData.setDescription(readStringDataset(group, "description"));
  channelData.setLocalTime(readStringDataset(group, "local_time"));
  channelData.setCountryCode(readStringDataset(group, "country_code"));
  channelData.setSystem(readStringDataset(group, "system"));
  channelData.setSoundSpeed(readMetadataTypeDataset(group, "sound_speed"));
  channelData.setRepetitionRate(readOptionalMetadataTypeDataset(group, "repetition_rate"));

  if (!skipData && !castData && group.openDataSet("data").getDataType() != H5DataType) {
    std::cerr << "Invalid format of data.\n";
    throw std::logic_error("Invalid format of data.");
  }
  // channel_data.data
  std::vector<hsize_t> dataDims;
  std::vector<DataType> dummy;

  readDataTypeArrayDataset(group, "data", skipData ? dummy : channelData.data(), dataDims,
                           H5DataType, skipData);

  if (dataDims.size() != 4) {
    std::cerr << "Dataset dimension != 4\n";
    throw std::logic_error("Dataset dimension != 4.");
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
  channelData.setUniqueEvents(
      readEventArray(uniqueEvents, channelData.probes(), channelData.uniqueWaves()));

  // Sequence
  H5::Group sequence(group.openGroup("sequence"));
  channelData.setSequence(readTimedEventArray(sequence, channelData.uniqueEvents()));

  return channelData;
}

template <typename DataType>
MetadataType Reader<DataType>::readMetadataTypeDataset(const H5::Group& group,
                                                       const std::string& name) {
  H5::StrType datatype(H5MetadataType);
  H5::DataSet dataset = group.openDataSet(name);
  MetadataType value;
  dataset.read(&value, datatype);
  return value;
}

template <typename DataType>
std::optional<MetadataType> Reader<DataType>::readOptionalMetadataTypeDataset(
    const H5::Group& group, const std::string& name) {
  H5::StrType datatype(H5MetadataType);
  H5::DataSet dataset = group.openDataSet(name);
  MetadataType value;
  dataset.read(&value, datatype);
  std::optional<MetadataType> result = std::nullopt;
  if (!std::isnan(value)) {
    result = value;
  }
  return result;
}

// template <typename DataType>
// std::optional<MetadataType> Reader<DataType>::readOptionalPosition2D(
//     const H5::Group& group, const std::string& name) {
//   H5::StrType datatype(H5MetadataType);
//   H5::DataSet dataset = group.openDataSet(name);
//     value;
//   dataset.read(&value, datatype);
//   std::optional<MetadataType> result = std::nullopt;
//   if (!std::isnan(value)) {
//     result = value;
//   }
//   return result;
// }

// template <typename DataType>
// uff::Element Reader<DataType>::readPosition2D(const H5::Group& group) {
//   uff::Point2D position{readMetadataTypeDataset(group, "x"), readMetadataTypeDataset(group, "y")};

//   return position;
// }

// template <typename DataType>
// uff::Element Reader<DataType>::readPosition3D(const H5::Group& group) {
//   uff::Point3D position{readMetadataTypeDataset(group, "x"), readMetadataTypeDataset(group, "y"),
//                            readMetadataTypeDataset(group, "z")};

//   return position;
// }

template <typename DataType>
std::optional<uff::Point3D<MetadataType>> Reader<DataType>::readElement(const H5::Group& /*group*/) {
  std::optional<uff::Point3D<MetadataType>> element;

  // element.setPosition(readMetadataTypeDataset(group, "x"));

  return element;
}

template <typename DataType>
std::vector<std::optional<uff::Point3D<MetadataType>>> Reader<DataType>::readElementArray(const H5::Group& group) {
  std::vector<std::optional<uff::Point3D<MetadataType>>> elements;

  char buf[9];
  int id = 1;
  while (true) {
    snprintf(buf, sizeof buf, "%08d", id);
    if (H5Lexists(group.getLocId(), buf, H5P_DEFAULT)) {
      H5::Group element = group.openGroup(buf);
      // elements.push_back(readElement(element));
      id++;
    } else {
      break;
    }
  }

  return elements;
}

template <typename DataType>
std::shared_ptr<uff::Event> Reader<DataType>::readEvent(
    const H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes,
    const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves) {
  auto event = std::make_shared<uff::Event>();

  // "receive_setup"
  event->setReceiveSetup(readReceiveSetup(group.openGroup("receive_setup"), probes));

  // "transmit_setup"
  event->setTransmitSetup(
      readTransmitSetup(group.openGroup("transmit_setup"), probes, uniqueWaves));

  return event;
}

template <typename DataType>
std::vector<std::shared_ptr<uff::Event>> Reader<DataType>::readEventArray(
    const H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes,
    const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves) {
  std::vector<std::shared_ptr<uff::Event>> events;

  char buf[9];
  int id = 1;
  while (true) {
    snprintf(buf, sizeof buf, "%08d", id);

    if (H5Lexists(group.getLocId(), buf, H5P_DEFAULT)) {
      H5::Group event = group.openGroup(buf);
      events.push_back(readEvent(event, probes, uniqueWaves));
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
                                                std::vector<hsize_t>& dimensions,
                                                const H5::PredType& targetType,
                                                bool skipChannelData) {
  H5::DataSet dataset = group.openDataSet(name);
  H5::StrType datatype(targetType);

  // find dataset dimensions
  H5::DataSpace dataspace = dataset.getSpace();
  int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(dimensions.data());
  size_t numel = 1;
  for (auto sz : dimensions) {
    numel *= sz;
    //std::cout << "sz:" << sz << std::endl;
  }

  if (!skipChannelData) {
    // reserve space in the output buffer
    values.resize(numel);

    // read data
    dataset.read(values.data(), datatype);
  }
}

template <typename DataType>
void Reader<DataType>::readMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                                    std::vector<MetadataType>& values,
                                                    std::vector<hsize_t>& dimensions) {
  H5::DataSet dataset = group.openDataSet(name);
  // TODO: check if type is correct : dataset.getTypeClass()
  H5::StrType datatype(H5MetadataType);

  // find dataset dimensions
  H5::DataSpace dataspace = dataset.getSpace();
  int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(dimensions.data());
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
                                               std::vector<hsize_t>& dimensions) {
  H5::DataSet dataset = group.openDataSet(name);
  // TODO: check if type is correct : dataset.getTypeClass()
  H5::StrType datatype(H5::PredType::NATIVE_INT);

  // find dataset dimensions
  H5::DataSpace dataspace = dataset.getSpace();
  int ndims = dataspace.getSimpleExtentNdims();
  //std::cout << "ndims:" << ndims << std::endl;
  dimensions.resize(ndims);
  dataspace.getSimpleExtentDims(dimensions.data());
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
  return value;
}

template <typename DataType>
std::shared_ptr<uff::LinearArray> Reader<DataType>::readLinearArray(const H5::Group& group) {
  auto linearArray =
      std::make_shared<uff::LinearArray>(readIntegerDataset(group, "number_elements"));

  // Read "pitch"
  linearArray->setPitch(readMetadataTypeDataset(group, "pitch"));

  // // Read "element_width"
  // linearArray->setElementWidth(readOptionalMetadataTypeDataset(group, "element_width"));

  // // Read "element_height"
  // linearArray->setElementHeight(readOptionalMetadataTypeDataset(group, "element_height"));

  return linearArray;
}

template <typename DataType>
std::shared_ptr<uff::MatrixArray> Reader<DataType>::readMatrixArray(const H5::Group& /*group*/) {
  auto matrixArray = std::make_shared<uff::MatrixArray>();

  // Read "number_elements"
  // matrixArray->setNumberElementsX(readIntegerDataset(group, "number_elements_x"));
  // matrixArray->setNumberElementsY(readIntegerDataset(group, "number_elements_y"));

  // // Read "pitch"
  // matrixArray->setPitchX(readMetadataTypeDataset(group, "pitch_x"));
  // matrixArray->setPitchY(readMetadataTypeDataset(group, "pitch_y"));

  // // Read "element_width"
  // matrixArray->setElementWidth(readOptionalMetadataTypeDataset(group, "element_width"));

  // // Read "element_height"
  // matrixArray->setElementHeight(readOptionalMetadataTypeDataset(group, "element_height"));

  return matrixArray;
}

template <typename DataType>
std::shared_ptr<RcaArray> Reader<DataType>::readRcaArray(const H5::Group& /*group*/) {
  auto rcaArray = std::make_shared<uff::RcaArray>(Point2D<uint32_t>()/*readIntegerDataset(group, "number_elements_x"),
                                                  readIntegerDataset(group, "number_elements_y")*/);

  // Read "pitch"
  // rcaArray->setPitchX(readMetadataTypeDataset(group, "pitch_x"));
  // rcaArray->setPitchY(readMetadataTypeDataset(group, "pitch_y"));

  // // Read "element_width"
  // rcaArray->setElementWidthX(readOptionalMetadataTypeDataset(group, "element_width_x"));
  // rcaArray->setElementWidthY(readOptionalMetadataTypeDataset(group, "element_width_y"));

  // // Read "element_height"
  // rcaArray->setElementHeightX(readOptionalMetadataTypeDataset(group, "element_height_x"));
  // rcaArray->setElementHeightY(readOptionalMetadataTypeDataset(group, "element_height_y"));

  return rcaArray;
}

template <typename DataType>
std::shared_ptr<uff::Probe> Reader<DataType>::readProbe(const H5::Group& group) {
  // probe_type (optional)
  std::shared_ptr<uff::Probe> probe(nullptr);
  if (H5Lexists(group.getLocId(), "probe_type", H5P_DEFAULT)) {
    const int probeType = readIntegerDataset(group, "probe_type");
    switch (ProbeType(probeType)) {
      case ProbeType::LinearArray: {
        auto linearArray = readLinearArray(group);
        linearArray->setTransform(probe->transform());
        linearArray->setFocalLength(probe->focalLength());
        linearArray->setElements(probe->elements());
        linearArray->setElementGeometries(probe->elementGeometries());
        linearArray->setImpulseResponses(probe->impulseResponses());
        probe = linearArray;
        break;
      }
      case ProbeType::MatrixArray: {
        auto matrixArray = readMatrixArray(group);
        matrixArray->setTransform(probe->transform());
        matrixArray->setFocalLength(probe->focalLength());
        matrixArray->setElements(probe->elements());
        matrixArray->setElementGeometries(probe->elementGeometries());
        matrixArray->setImpulseResponses(probe->impulseResponses());
        probe = matrixArray;
        break;
      }
      case ProbeType::RcaArray: {
        auto rcaArray = readRcaArray(group);
        rcaArray->setTransform(probe->transform());
        rcaArray->setFocalLength(probe->focalLength());
        rcaArray->setElements(probe->elements());
        rcaArray->setElementGeometries(probe->elementGeometries());
        rcaArray->setImpulseResponses(probe->impulseResponses());
        probe = rcaArray;
        break;
      }
      default: {
        std::cerr << ": Ignoring unknown probe_type:" << probeType << "\n";
        throw std::logic_error("unknown probe_type.");
      }
    }
  } else {
    std::cerr << "No probe_type\n";
    throw std::logic_error("No probe_type");
  }

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
uff::ReceiveSetup Reader<DataType>::readReceiveSetup(
    const H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes) {
  uff::ReceiveSetup receiveSetup;

  // "probe"
  int probeId = std::stoi(readStringDataset(group, "probe_id"));
  receiveSetup.setProbe(probes[static_cast<size_t>(probeId) - 1]);

  // "time_offset"
  receiveSetup.setTimeOffset(readMetadataTypeDataset(group, "time_offset"));

  // "sampling_frequency"
  receiveSetup.setSamplingFrequency(readMetadataTypeDataset(group, "sampling_frequency"));

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
    std::vector<hsize_t> dimensions;
    readIntegerArrayDataset(group, "channel_mapping", channelMapping, dimensions);
    receiveSetup.setChannelMapping(channelMapping);
  }

  // "tgc_profile" [optional]
  if (H5Lexists(group.getLocId(), "tgc_profile", H5P_DEFAULT)) {
    std::vector<MetadataType> tgcProfile;
    std::vector<hsize_t> dimensions;
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
uff::Point3D<MetadataType> Reader<DataType>::readRotation(const H5::Group& group) {
  uff::Point3D<MetadataType> rotation{readMetadataTypeDataset(group, "x"),
                                      readMetadataTypeDataset(group, "y"),
                                      readMetadataTypeDataset(group, "z")};

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
uff::TimedEvent Reader<DataType>::readTimedEvent(
    const H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents) {
  uff::TimedEvent timedEvent;

  // "event"
  int eventId = std::stoi(readStringDataset(group, "event_id"));
  //std::cout << "probeId" << probeId << " " << _dataset.channelData().probes().size();
  timedEvent.setEvent(uniqueEvents[static_cast<size_t>(eventId) - 1]);

  // "time_offset"
  timedEvent.setTimeOffset(readMetadataTypeDataset(group, "time_offset"));

  return timedEvent;
}

template <typename DataType>
std::vector<uff::TimedEvent> Reader<DataType>::readTimedEventArray(
    const H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents) {
  std::vector<uff::TimedEvent> timedEvents;

  char buf[9];
  int id = 1;
  while (true) {
    snprintf(buf, sizeof buf, "%08d", id);

    if (H5Lexists(group.getLocId(), buf, H5P_DEFAULT)) {
      H5::Group timedEvent = group.openGroup(buf);
      timedEvents.push_back(readTimedEvent(timedEvent, uniqueEvents));
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
  uff::Point3D<MetadataType> rotation = readRotation(group.openGroup("rotation"));

  // translation
  uff::Point3D<MetadataType> translation = readTranslation(group.openGroup("translation"));

  return {rotation, translation};
}

template <typename DataType>
uff::Point3D<MetadataType> Reader<DataType>::readTranslation(const H5::Group& group) {
  uff::Point3D<MetadataType> translation{readMetadataTypeDataset(group, "x"),
                                         readMetadataTypeDataset(group, "y"),
                                         readMetadataTypeDataset(group, "z")};

  return translation;
}

template <typename DataType>
uff::TransmitSetup Reader<DataType>::readTransmitSetup(
    const H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes,
    const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves) {
  uff::TransmitSetup transmitSetup;

  // "probe"
  int probeId = std::stoi(readStringDataset(group, "probe_id"));
  transmitSetup.setProbe(probes[static_cast<size_t>(probeId) - 1]);

  // "transmit_wave"
  transmitSetup.setTransmitWave(readTransmitWave(group.openGroup("transmit_wave"), uniqueWaves));

  // channel_mapping [optional]
  if (H5Lexists(group.getLocId(), "channel_mapping", H5P_DEFAULT)) {
    std::vector<int> channelMapping;
    std::vector<hsize_t> dimensions;
    readIntegerArrayDataset(group, "channel_mapping", channelMapping, dimensions);
    transmitSetup.setChannelMapping(channelMapping);
  }

  return transmitSetup;
}

template <typename DataType>
uff::TransmitWave Reader<DataType>::readTransmitWave(
    const H5::Group& group, const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves) {
  uff::TransmitWave transmitWave;

  // "wave"
  int waveId = std::stoi(readStringDataset(group, "wave_id"));
  transmitWave.setWave(uniqueWaves[static_cast<size_t>(waveId) - 1]);

  // "time_offset"
  transmitWave.setTimeOffset(readMetadataTypeDataset(group, "time_offset"));

  // "weight"
  transmitWave.setWeight(readMetadataTypeDataset(group, "weight"));

  return transmitWave;
}

template <typename DataType>
uff::Version Reader<DataType>::readVersion(const H5::Group& group) {
  int major = readIntegerDataset(group, "major");
  int minor = readIntegerDataset(group, "minor");
  int patch = readIntegerDataset(group, "patch");
  return uff::Version(major, minor, patch);
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
