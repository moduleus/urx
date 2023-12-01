/*!
 * Copyright Moduleus
 * \file urx/writer.cpp
 * \brief
 */

#ifdef WITH_HDF5

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <ios>
#include <limits>
#include <ostream>
#include <type_traits>

#include <urx/v0_5/aperture.h>
#include <urx/v0_5/channel_data.h>
#include <urx/v0_5/dataset.h>
#include <urx/v0_5/element.h>
#include <urx/v0_5/event.h>
#include <urx/v0_5/excitation.h>
#include <urx/v0_5/linear_array.h>
#include <urx/v0_5/matrix_array.h>
#include <urx/v0_5/object.h>
#include <urx/v0_5/probe.h>
#include <urx/v0_5/rca_array.h>
#include <urx/v0_5/receive_setup.h>
#include <urx/v0_5/rotation.h>
#include <urx/v0_5/timed_event.h>
#include <urx/v0_5/transform.h>
#include <urx/v0_5/translation.h>
#include <urx/v0_5/transmit_setup.h>
#include <urx/v0_5/transmit_wave.h>
#include <urx/v0_5/urx.h>
#include <urx/v0_5/version.h>
#include <urx/v0_5/wave.h>
#include <urx/v0_5/writer.h>

#define H5DataType \
  (std::is_same_v<DataType, float> ? H5::PredType::NATIVE_FLOAT : H5::PredType::NATIVE_SHORT)
#define H5MetadataType \
  (std::is_same_v<MetadataType, float> ? H5::PredType::NATIVE_FLOAT : H5::PredType::NATIVE_DOUBLE)

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif

namespace urx::v0_5 {

/**
 * Return the string-ID (format: "00000123") of the pointer wptr.
 * The string-ID is the position of the pointer in the vector 'vec'. First pointer's string-ID is "00000001".
 * If 'wptr' does not point to an object stored in 'vec' then return "????????"
 */
template <typename DataType>
template <typename T>
std::string Writer<DataType>::getIdFromPointer(const std::vector<std::shared_ptr<T>>& vec,
                                               std::weak_ptr<T> wptr) {
  if (auto p1 = wptr.lock()) {
    int cnt = 1;
    for (const auto& p2 : vec) {
      if (p1 == p2) {
        char buf[9];
        snprintf(buf, sizeof buf, "%08d", cnt);
        return {buf};
      }
      cnt++;
    }
  }

  assert(false);
  return "????????";
}

template <typename DataType>
void Writer<DataType>::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "HDF5 Version: "
     << "TODO" << '\n'
     << indent << "URX Version: "
     << "TODO" << '\n'
     << indent << "FileName: " << this->m_fileName << '\n'
     << indent << "Dataset: " << *this->m_dataset << '\n';
}

template <typename DataType>
void Writer<DataType>::writeToFile() {
  H5::Exception::dontPrint();

  const H5::H5File file(m_fileName, H5F_ACC_TRUNC);

  // Version
  H5::Group version(file.createGroup("version"));
  writeVersion(version, m_dataset->version());

  // Channel Data
  H5::Group channelData(file.createGroup("channel_data"));
  writeChannelData(channelData, m_dataset->channelData());
}

template <typename DataType>
void Writer<DataType>::writeAperture(H5::Group& group, const Aperture& aperture) {
  // "origin"
  H5::Group origin = group.createGroup("origin");
  writeTransform(origin, aperture.origin());

  // "window"
  writeOptionalStringDataset(group, "window", aperture.window());

  // "f_number"
  writeOptionalMetadataTypeDataset(group, "f_number", aperture.fNumber());

  // "fixed_size"
  writeOptionalMetadataTypeDataset(group, "fixed_size", aperture.fixedSize());

  // "maximum_size" TODO
  // "minimum_size" TODO
}

template <typename DataType>
void Writer<DataType>::writeChannelData(H5::Group& group,
                                        const ChannelData<DataType>& channelData) {
  // channel_data.authors
  writeStringDataset(group, "authors", channelData.authors());

  // channel_data.description
  writeStringDataset(group, "description", channelData.description());

  // channel_data.local_time
  writeStringDataset(group, "local_time", channelData.localTime());

  // channel_data.country_code
  writeStringDataset(group, "country_code", channelData.countryCode());

  // channel_data.system
  writeStringDataset(group, "system", channelData.system());

  // channel_data.sound_speed
  writeOptionalMetadataTypeDataset(group, "sound_speed", channelData.soundSpeed());

  // channel_data.repetition_rate
  writeOptionalMetadataTypeDataset(group, "repetition_rate", channelData.repetitionRate());

  // channel_data.data
  const size_t nFrames = channelData.numberOfFrames();
  const size_t nEvents = channelData.numberOfEvents();
  const size_t nChannels = channelData.numberOfChannels();
  const size_t nSamples = channelData.numberOfSamples();
  const std::vector<size_t> dims({nFrames, nEvents, nChannels, nSamples});
  writeDataTypeArrayDataset(group, "data", m_dataset->channelData().data(), dims);

  // Probes
  H5::Group probes(group.createGroup("probes"));
  writeProbeArray(probes, channelData.probes());

  // Unique waves
  H5::Group waves(group.createGroup("unique_waves"));
  writeWaveArray(waves, channelData.uniqueWaves());

  // Unique events
  H5::Group uniqueEvents(group.createGroup("unique_events"));
  writeEventArray(uniqueEvents, channelData.uniqueEvents());

  // Sequence
  H5::Group sequence(group.createGroup("sequence"));
  writeTimedEventArray(sequence, channelData.sequence());
}

template <typename DataType>
void Writer<DataType>::writeMetadataTypeDataset(H5::Group& group, const std::string& name,
                                                MetadataType value) {
  const H5::StrType datatype(H5MetadataType);
  const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(&value, datatype, dataspace);
}

template <typename DataType>
void Writer<DataType>::writeOptionalMetadataTypeDataset(H5::Group& group, const std::string& name,
                                                        const std::optional<MetadataType>& value) {
  const H5::StrType datatype(H5MetadataType);
  const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  if (value.has_value()) {
    dataset.write(&value.value(), datatype, dataspace);
  } else {
    constexpr MetadataType nan = std::numeric_limits<MetadataType>::quiet_NaN();
    dataset.write(&nan, datatype, dataspace);
  }
}

template <typename DataType>
void Writer<DataType>::writeElement(H5::Group& group, const Element& element) {
  writeOptionalMetadataTypeDataset(group, "x", element.x());
  writeOptionalMetadataTypeDataset(group, "y", element.y());
  writeOptionalMetadataTypeDataset(group, "z", element.z());
}

template <typename DataType>
void Writer<DataType>::writeElementArray(H5::Group& group, const std::vector<Element>& elements) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < elements.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    const std::string element_id = buf;
    H5::Group element = group.createGroup(element_id);
    writeElement(element, elements[i]);
  }
}

template <typename DataType>
void Writer<DataType>::writeEvent(H5::Group& group, const std::shared_ptr<Event>& ev) {
  // "transmit_setup"
  H5::Group transmitSetup = group.createGroup("transmit_setup");
  writeTransmitSetup(transmitSetup, ev->transmitSetup());

  // "receive_setup"
  H5::Group receiveSetup = group.createGroup("receive_setup");
  writeReceiveSetup(receiveSetup, ev->receiveSetup());
}

template <typename DataType>
void Writer<DataType>::writeEventArray(H5::Group& group,
                                       const std::vector<std::shared_ptr<Event>>& events) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < events.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    const std::string event_id = buf;
    H5::Group ev = group.createGroup(event_id);
    writeEvent(ev, events[i]);
  }
}

template <typename DataType>
void Writer<DataType>::writeExcitation(H5::Group& group, const Excitation& excitation) {
  // "pulse_shape"
  writeOptionalStringDataset(group, "pulse_shape", excitation.pulseShape());

  // "transmit_frequency"
  writeOptionalMetadataTypeDataset(group, "transmit_frequency", excitation.transmitFrequency());

  // "waveform"
  writeMetadataTypeArrayDataset(group, "waveform", excitation.waveform(), {});

  // "sampling_frequency"
  writeOptionalMetadataTypeDataset(group, "sampling_frequency", excitation.samplingFrequency());
}

template <typename DataType>
void Writer<DataType>::writeDataTypeArrayDataset(H5::Group& group, const std::string& name,
                                                 const std::vector<DataType>& values,
                                                 const std::vector<size_t>& dimensions) {
  assert(dimensions.size() <= 4);

  hsize_t dims[4];
  size_t ndims = dimensions.size();
  if (ndims == 0)  // unspecified dimension: write 1D array
  {
    ndims = 1;
    dims[0] = values.size();
  } else {
#ifndef NDEBUG
    size_t numel = dimensions[0];
#endif
    dims[0] = dimensions[0];
    for (uint32_t i = 1; i < dimensions.size(); i++) {
#ifndef NDEBUG
      numel *= dimensions[i];
#endif
      dims[i] = dimensions[i];
    }

    // check if prod(dimensions) == values.length()
    assert(values.size() == numel);
  }

  const H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  const H5::DataSet dataset = group.createDataSet(name, H5DataType, dataspace);
  dataset.write(values.data(), H5DataType);
}

template <typename DataType>
void Writer<DataType>::writeMetadataTypeArrayDataset(H5::Group& group, const std::string& name,
                                                     const std::vector<MetadataType>& values,
                                                     const std::vector<size_t>& dimensions) {
  assert(dimensions.size() <= 4);

  hsize_t dims[4];
  size_t ndims = dimensions.size();
  if (ndims == 0)  // unspecified dimension: write 1D array
  {
    ndims = 1;
    dims[0] = values.size();
  } else {
#ifndef NDEBUG
    size_t numel = dimensions[0];
#endif
    dims[0] = dimensions[0];
    for (uint32_t i = 1; i < dimensions.size(); i++) {
#ifndef NDEBUG
      numel *= dimensions[i];
#endif
      dims[i] = dimensions[i];
    }

    // check if prod(dimensions) == values.length()
    assert(values.size() == numel);
  }

  const H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  const H5::DataSet dataset = group.createDataSet(name, H5MetadataType, dataspace);
  dataset.write(values.data(), H5MetadataType);
}

template <typename DataType>
void Writer<DataType>::writeIntegerArrayDataset(H5::Group& group, const std::string& name,
                                                const std::vector<int>& values,
                                                const std::vector<size_t>& dimensions) {
  assert(dimensions.size() <= 4);

  hsize_t dims[4];
  size_t ndims = dimensions.size();
  if (ndims == 0)  // unspecified dimension: write 1D array
  {
    ndims = 1;
    dims[0] = values.size();
  } else {
#ifndef NDEBUG
    size_t numel = dimensions[0];
#endif
    dims[0] = dimensions[0];
    for (uint32_t i = 1; i < dimensions.size(); i++) {
#ifndef NDEBUG
      numel *= dimensions[i];
#endif
      dims[i] = dimensions[i];
    }

    // check if prod(dimensions) == values.length()
    assert(values.size() == numel);
  }

  const H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  const H5::DataSet dataset = group.createDataSet(name, H5::PredType::NATIVE_INT, dataspace);
  dataset.write(values.data(), H5::PredType::NATIVE_INT);
}

template <typename DataType>
void Writer<DataType>::writeIntegerDataset(H5::Group& group, const std::string& name, int value) {
  const H5::StrType datatype(H5::PredType::NATIVE_INT);
  const H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(&value, datatype, dataspace);
}

template <typename DataType>
void Writer<DataType>::writeLinearArray(H5::Group& group,
                                        const std::shared_ptr<LinearArray>& linearArray) {
  // Write "number_elements"
  writeIntegerDataset(group, "number_elements", linearArray->numberElements());

  // Write "pitch"
  writeMetadataTypeDataset(group, "pitch", linearArray->pitch());

  // Write "element_width"
  writeOptionalMetadataTypeDataset(group, "element_width", linearArray->elementWidth());

  // Write "element_height"
  writeOptionalMetadataTypeDataset(group, "element_height", linearArray->elementHeight());
}

template <typename DataType>
void Writer<DataType>::writeMatrixArray(H5::Group& group,
                                        const std::shared_ptr<MatrixArray>& matrixArray) {
  // Write "number_elements_x"
  writeIntegerDataset(group, "number_elements_x", matrixArray->numberElementsX());

  // Write "number_elements_y"
  writeIntegerDataset(group, "number_elements_y", matrixArray->numberElementsY());

  // Write "pitch_x"
  writeMetadataTypeDataset(group, "pitch_x", matrixArray->pitchX());

  // Write "pitch_y"
  writeMetadataTypeDataset(group, "pitch_y", matrixArray->pitchY());

  // Write "element_width"
  writeOptionalMetadataTypeDataset(group, "element_width", matrixArray->elementWidth());

  // Write "element_height"
  writeOptionalMetadataTypeDataset(group, "element_height", matrixArray->elementHeight());
}

template <typename DataType>
void Writer<DataType>::writeRcaArray(H5::Group& group, const std::shared_ptr<RcaArray>& rcaArray) {
  // Write "number_elements_x"
  writeIntegerDataset(group, "number_elements_x", rcaArray->numberElementsX());

  // Write "number_elements_y"
  writeIntegerDataset(group, "number_elements_y", rcaArray->numberElementsY());

  // Write "pitch_x"
  writeMetadataTypeDataset(group, "pitch_x", rcaArray->pitchX());

  // Write "pitch_y"
  writeMetadataTypeDataset(group, "pitch_y", rcaArray->pitchY());

  // Write "element_width_x"
  writeOptionalMetadataTypeDataset(group, "element_width_x", rcaArray->elementWidthX());

  // Write "element_width_y"
  writeOptionalMetadataTypeDataset(group, "element_width_y", rcaArray->elementWidthY());

  // Write "element_height_x"
  writeOptionalMetadataTypeDataset(group, "element_height_x", rcaArray->elementHeightX());

  // Write "element_height_y"
  writeOptionalMetadataTypeDataset(group, "element_height_y", rcaArray->elementHeightY());
}

template <typename DataType>
void Writer<DataType>::writeProbe(H5::Group& group, const std::shared_ptr<Probe>& probe) {
  // write "transform"
  H5::Group transform = group.createGroup("transform");
  writeTransform(transform, probe->transform());

  // write "elements"
  H5::Group elements = group.createGroup("elements");
  writeElementArray(elements, probe->elements());

  // write "focal_length" (optional)
  writeOptionalMetadataTypeDataset(group, "focal_length", probe->focalLength());

  // MatrixArray ?
  const std::shared_ptr<MatrixArray> matrixArray = std::dynamic_pointer_cast<MatrixArray>(probe);
  if (matrixArray != nullptr) {
    writeStringDataset(group, "probe_type", "MatrixArray");
    writeMatrixArray(group, matrixArray);
    return;
  }

  // LinearArray ?
  const std::shared_ptr<LinearArray> linearArray = std::dynamic_pointer_cast<LinearArray>(probe);
  if (linearArray != nullptr) {
    writeStringDataset(group, "probe_type", "LinearArray");
    writeLinearArray(group, linearArray);
    return;
  }

  // RcaArray ?
  const std::shared_ptr<RcaArray> rcaArray = std::dynamic_pointer_cast<RcaArray>(probe);
  if (rcaArray != nullptr) {
    writeStringDataset(group, "probe_type", "RcaArray");
    writeRcaArray(group, rcaArray);
    return;
  }
}

template <typename DataType>
void Writer<DataType>::writeProbeArray(H5::Group& group,
                                       const std::vector<std::shared_ptr<Probe>>& probes) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < probes.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    const std::string probe_id = buf;
    H5::Group probe = group.createGroup(probe_id);
    writeProbe(probe, probes[i]);
  }
}

template <typename DataType>
void Writer<DataType>::writeReceiveSetup(H5::Group& group, const ReceiveSetup& receiveSetup) {
  // "probe"
  const std::string probeId =
      getIdFromPointer<Probe>(m_dataset->channelData().probes(), receiveSetup.probe());
  writeStringDataset(group, "probe_id", probeId);

  // "time_offset"
  writeMetadataTypeDataset(group, "time_offset", receiveSetup.timeOffset());

  // "sampling_frequency"
  writeOptionalMetadataTypeDataset(group, "sampling_frequency", receiveSetup.samplingFrequency());

  // "sampling_type"
  writeIntegerDataset(group, "sampling_type", static_cast<int>(receiveSetup.samplingType()));

  // "channel_mapping"
  writeIntegerArrayDataset(group, "channel_mapping", receiveSetup.channelMapping(), {});

  // "tgc_profile"
  writeMetadataTypeArrayDataset(group, "tgc_profile", receiveSetup.tgcProfile(), {});

  // "tgc_sampling_frequency"
  writeOptionalMetadataTypeDataset(group, "tgc_sampling_frequency",
                                   receiveSetup.tgcSamplingFrequency());

  // "modulation_frequency"
  writeOptionalMetadataTypeDataset(group, "modulation_frequency",
                                   receiveSetup.modulationFrequency());
}

template <typename DataType>
void Writer<DataType>::writeRotation(H5::Group& group, const Rotation& rotation) {
  writeMetadataTypeDataset(group, "x", rotation.x());
  writeMetadataTypeDataset(group, "y", rotation.y());
  writeMetadataTypeDataset(group, "z", rotation.z());
}

template <typename DataType>
void Writer<DataType>::writeStringDataset(H5::Group& group, const std::string& name,
                                          const std::string& value) {
  const H5::StrType vlst(0, H5T_VARIABLE);
  const H5::DataSpace ds_space(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name.c_str(), vlst, ds_space);
  dataset.write(value, vlst);
}

template <typename DataType>
void Writer<DataType>::writeOptionalStringDataset(H5::Group& group, const std::string& name,
                                                  const std::optional<std::string>& value) {
  const H5::StrType vlst(0, H5T_VARIABLE);
  const H5::DataSpace ds_space(H5S_SCALAR);
  const H5::DataSet dataset = group.createDataSet(name.c_str(), vlst, ds_space);
  if (value.has_value())
    dataset.write(value.value(), vlst);
  else
    dataset.write(std::string("undefined"), vlst);
}

template <typename DataType>
void Writer<DataType>::writeTimedEvent(H5::Group& group, const TimedEvent& timedEvent) {
  // "event"
  const std::string eventId =
      getIdFromPointer<Event>(m_dataset->channelData().uniqueEvents(), timedEvent.evenement());
  writeStringDataset(group, "event_id", eventId);

  // "time_offset"
  writeMetadataTypeDataset(group, "time_offset", timedEvent.timeOffset());
}

template <typename DataType>
void Writer<DataType>::writeTimedEventArray(H5::Group& group,
                                            const std::vector<TimedEvent>& timedEvents) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < timedEvents.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    const std::string timedEvent_id = buf;
    H5::Group timedEvent = group.createGroup(timedEvent_id);
    writeTimedEvent(timedEvent, timedEvents[i]);
  }
}

template <typename DataType>
void Writer<DataType>::writeTransform(H5::Group& group, const Transform& transform) {
  // "rotation"
  H5::Group rotation = group.createGroup("rotation");
  writeRotation(rotation, transform.rotation());

  // "translation"
  H5::Group translation = group.createGroup("translation");
  writeTranslation(translation, transform.translation());
}

template <typename DataType>
void Writer<DataType>::writeTranslation(H5::Group& group, const Translation& translation) {
  writeMetadataTypeDataset(group, "x", translation.x());
  writeMetadataTypeDataset(group, "y", translation.y());
  writeMetadataTypeDataset(group, "z", translation.z());
}

template <typename DataType>
void Writer<DataType>::writeTransmitSetup(H5::Group& group, const TransmitSetup& transmitSetup) {
  // "probe"
  const std::string probeId =
      getIdFromPointer<Probe>(m_dataset->channelData().probes(), transmitSetup.probe());
  writeStringDataset(group, "probe_id", probeId);

  // "transmit_wave"
  H5::Group transmitWave = group.createGroup("transmit_wave");
  writeTransmitWave(transmitWave, transmitSetup.getTransmitWave());

  // "channel_mapping"
  writeIntegerArrayDataset(group, "channel_mapping", transmitSetup.channelMapping(), {});
}

template <typename DataType>
void Writer<DataType>::writeTransmitWave(H5::Group& group, const TransmitWave& transmitWave) {
  // "wave"
  const std::string waveId =
      getIdFromPointer<Wave>(m_dataset->channelData().uniqueWaves(), transmitWave.wave());
  writeStringDataset(group, "wave_id", waveId);

  // "time_offset"
  writeMetadataTypeDataset(group, "time_offset", transmitWave.timeOffset());

  // "weight"
  writeMetadataTypeDataset(group, "weight", transmitWave.weight());
}

template <typename DataType>
void Writer<DataType>::writeVersion(H5::Group& group, const Version& version) {
  writeIntegerDataset(group, "major", version.major());
  writeIntegerDataset(group, "minor", version.minor());
  writeIntegerDataset(group, "patch", version.patch());
}

template <typename DataType>
void Writer<DataType>::writeWave(H5::Group& group, const std::shared_ptr<Wave>& wave) {
  // write "origin"
  H5::Group origin = group.createGroup("origin");
  writeTransform(origin, wave->origin());

  // write "wave_type"
  writeIntegerDataset(group, "wave_type", static_cast<int>(wave->waveType()));

  // write "aperture"
  H5::Group aperture = group.createGroup("aperture");
  writeAperture(aperture, wave->aperture());

  // write "excitation"
  H5::Group excitation = group.createGroup("excitation");
  writeExcitation(excitation, wave->excitation());
}

template <typename DataType>
void Writer<DataType>::writeWaveArray(H5::Group& group,
                                      const std::vector<std::shared_ptr<Wave>>& waves) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < waves.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    const std::string wave_id = buf;
    H5::Group wave = group.createGroup(wave_id);
    writeWave(wave, waves[i]);
  }
}

template class Writer<float>;
template class Writer<short>;

}  // namespace urx::v0_5

#endif  // WITH_HDF5
