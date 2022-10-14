/*!
 * Copyright Moduleus
 * \file uff/writer.cpp
 * \brief
 */

#ifdef WITH_HDF5
#include "uff/writer.h"

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif

namespace uff {

/**
 * Return the string-ID (format: "00000123") of the pointer wptr.
 * The string-ID is the position of the pointer in the vector 'vec'. First pointer's string-ID is "00000001".
 * If 'wptr' does not point to an object stored in 'vec' then return "????????"
 */
template <typename T>
std::string Writer::getIdFromPointer(const std::vector<std::shared_ptr<T>>& vec,
                                     std::weak_ptr<T> wptr) {
  if (auto p1 = wptr.lock()) {
    int cnt = 1;
    for (auto p2 : vec) {
      if (p1 == p2) {
        char buf[9];
        snprintf(buf, sizeof buf, "%08d", cnt);
        return std::string(buf);
      }
      cnt++;
    }
  }

  assert(false);
  return "????????";
}

void Writer::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
  os << indent << "HDF5 Version: "
     << "TODO" << std::endl
     << indent << "UFF Version: "
     << "TODO" << std::endl
     << indent << "FileName: " << this->m_fileName << std::endl
     << indent << "Dataset: " << this->m_dataset << std::endl;
}

bool Writer::writeToFile() {
  try {
    H5::Exception::dontPrint();

    H5::H5File file(m_fileName, H5F_ACC_TRUNC);

    // Version
    H5::Group version(file.createGroup("version"));
    writeVersion(version, m_dataset.version());

    // Channel Data
    H5::Group channelData(file.createGroup("channel_data"));
    writeChannelData(channelData, m_dataset.channelData());

    file.close();

    return true;
  }  // end of try block
  // catch failure caused by the H5File operations
  catch (const H5::FileIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return false;
  }
  // catch failure caused by the DataSet operations
  catch (const H5::DataSetIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return false;
  }
  // catch failure caused by the DataSpace operations
  catch (const H5::DataSpaceIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return false;
  }
  // catch failure caused by the DataSpace operations
  catch (const H5::DataTypeIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return false;
  }
  // catch failure caused by the Group operations
  catch (const H5::GroupIException& error) {
    error.printErrorStack();
    std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
    return false;
  }
}

void Writer::writeAperture(H5::Group& group, const uff::Aperture& aperture) {
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

void Writer::writeChannelData(H5::Group& group, const uff::ChannelData& channelData) {
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
  size_t nFrames = channelData.numberOfFrames();
  size_t nEvents = channelData.numberOfEvents();
  size_t nChannels = channelData.numberOfChannels();
  size_t nSamples = channelData.numberOfSamples();
  std::vector<size_t> dims({nFrames, nEvents, nChannels, nSamples});
  writeDataTypeArrayDataset(group, "data", m_dataset.channelData().data(), dims);

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

H5::DataSet Writer::writeMetadataTypeDataset(H5::Group& group, const std::string& name,
                                             MetadataType value) {
  H5::StrType datatype(H5MetadataType);
  H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(&value, datatype, dataspace);
  dataset.close();
  return dataset;
}

H5::DataSet Writer::writeOptionalMetadataTypeDataset(H5::Group& group, const std::string& name,
                                                     std::optional<MetadataType> value) {
  H5::StrType datatype(H5MetadataType);
  H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  if (value.has_value()) {
    dataset.write(&value.value(), datatype, dataspace);
  } else {
    constexpr MetadataType nan = std::numeric_limits<MetadataType>::quiet_NaN();
    dataset.write(&nan, datatype, dataspace);
  }
  dataset.close();
  return dataset;
}

void Writer::writeElement(H5::Group& group, const uff::Element& element) {
  writeOptionalMetadataTypeDataset(group, "x", element.x());
  writeOptionalMetadataTypeDataset(group, "y", element.y());
  writeOptionalMetadataTypeDataset(group, "z", element.z());
}

void Writer::writeElementArray(H5::Group& group, const std::vector<uff::Element>& elements) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < elements.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    std::string element_id = buf;
    H5::Group element = group.createGroup(element_id);
    writeElement(element, elements[i]);
  }
}

void Writer::writeEvent(H5::Group& group, const std::shared_ptr<uff::Event>& ev) {
  // "transmit_setup"
  H5::Group transmitSetup = group.createGroup("transmit_setup");
  writeTransmitSetup(transmitSetup, ev->transmitSetup());

  // "receive_setup"
  H5::Group receiveSetup = group.createGroup("receive_setup");
  writeReceiveSetup(receiveSetup, ev->receiveSetup());
}

void Writer::writeEventArray(H5::Group& group,
                             const std::vector<std::shared_ptr<uff::Event>>& events) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < events.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    std::string event_id = buf;
    H5::Group ev = group.createGroup(event_id);
    writeEvent(ev, events[i]);
  }
}

void Writer::writeExcitation(H5::Group& group, const uff::Excitation& excitation) {
  // "pulse_shape"
  writeOptionalStringDataset(group, "pulse_shape", excitation.pulseShape());

  // "transmit_frequency"
  writeOptionalMetadataTypeDataset(group, "transmit_frequency", excitation.transmitFrequency());

  // "waveform"
  writeMetadataTypeArrayDataset(group, "waveform", excitation.waveform(), {});

  // "sampling_frequency"
  writeOptionalMetadataTypeDataset(group, "sampling_frequency", excitation.samplingFrequency());
}

H5::DataSet Writer::writeDataTypeArrayDataset(H5::Group& group, const std::string& name,
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

  H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  H5::DataSet dataset = group.createDataSet(name, H5DataType, dataspace);
  dataset.write(values.data(), H5DataType);
  dataset.close();
  return dataset;
}

H5::DataSet Writer::writeMetadataTypeArrayDataset(H5::Group& group, const std::string& name,
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

  H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  H5::DataSet dataset = group.createDataSet(name, H5MetadataType, dataspace);
  dataset.write(values.data(), H5MetadataType);
  dataset.close();
  return dataset;
}

H5::DataSet Writer::writeIntegerArrayDataset(H5::Group& group, const std::string& name,
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

  H5::DataSpace dataspace = H5::DataSpace(static_cast<int>(ndims), dims);
  H5::DataSet dataset = group.createDataSet(name, H5::PredType::NATIVE_INT, dataspace);
  dataset.write(values.data(), H5::PredType::NATIVE_INT);
  dataset.close();
  return dataset;
}

H5::DataSet Writer::writeIntegerDataset(H5::Group& group, const std::string& name, int value) {
  H5::StrType datatype(H5::PredType::NATIVE_INT);
  H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
  H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
  dataset.write(&value, datatype, dataspace);
  dataset.close();
  return dataset;
}

void Writer::writeLinearArray(H5::Group& group,
                              const std::shared_ptr<uff::LinearArray>& linearArray) {
  // Write "number_elements"
  writeIntegerDataset(group, "number_elements", linearArray->numberElements());

  // Write "pitch"
  writeOptionalMetadataTypeDataset(group, "pitch", linearArray->pitch());

  // Write "element_width"
  writeOptionalMetadataTypeDataset(group, "element_width", linearArray->elementWidth());

  // Write "element_height"
  writeOptionalMetadataTypeDataset(group, "element_height", linearArray->elementHeight());
}

void Writer::writeMatrixArray(H5::Group& group,
                              const std::shared_ptr<uff::MatrixArray>& matrixArray) {
  // Write "number_elements_x"
  writeIntegerDataset(group, "number_elements_x", matrixArray->numberElementsX());

  // Write "number_elements_y"
  writeIntegerDataset(group, "number_elements_y", matrixArray->numberElementsY());

  // Write "pitch_x"
  writeOptionalMetadataTypeDataset(group, "pitch_x", matrixArray->pitchX());

  // Write "pitch_y"
  writeOptionalMetadataTypeDataset(group, "pitch_y", matrixArray->pitchY());

  // Write "element_width"
  writeOptionalMetadataTypeDataset(group, "element_width", matrixArray->elementWidth());

  // Write "element_height"
  writeOptionalMetadataTypeDataset(group, "element_height", matrixArray->elementHeight());
}

void Writer::writeRcaArray(H5::Group& group, const std::shared_ptr<uff::RcaArray>& rcaArray) {
  // Write "number_elements_x"
  writeIntegerDataset(group, "number_elements_x", rcaArray->numberElementsX());

  // Write "number_elements_y"
  writeIntegerDataset(group, "number_elements_y", rcaArray->numberElementsY());

  // Write "pitch_x"
  writeOptionalMetadataTypeDataset(group, "pitch_x", rcaArray->pitchX());

  // Write "pitch_y"
  writeOptionalMetadataTypeDataset(group, "pitch_y", rcaArray->pitchY());

  // Write "element_width_x"
  writeOptionalMetadataTypeDataset(group, "element_width_x", rcaArray->elementWidthX());

  // Write "element_width_y"
  writeOptionalMetadataTypeDataset(group, "element_width_y", rcaArray->elementWidthY());

  // Write "element_height_x"
  writeOptionalMetadataTypeDataset(group, "element_height_x", rcaArray->elementHeightX());

  // Write "element_height_y"
  writeOptionalMetadataTypeDataset(group, "element_height_y", rcaArray->elementHeightY());
}

void Writer::writeProbe(H5::Group& group, const std::shared_ptr<uff::Probe>& probe) {
  // write "transform"
  H5::Group transform = group.createGroup("transform");
  writeTransform(transform, probe->transform());

  // write "elements"
  H5::Group elements = group.createGroup("elements");
  writeElementArray(elements, probe->elements());

  // write "focal_length" (optional)
  writeOptionalMetadataTypeDataset(group, "focal_length", probe->focalLength());

  // MatrixArray ?
  std::shared_ptr<uff::MatrixArray> matrixArray =
      std::dynamic_pointer_cast<uff::MatrixArray>(probe);
  if (matrixArray.get() != nullptr) {
    writeStringDataset(group, "probe_type", "MatrixArray");
    writeMatrixArray(group, matrixArray);
    return;
  }

  // LinearArray ?
  std::shared_ptr<uff::LinearArray> linearArray =
      std::dynamic_pointer_cast<uff::LinearArray>(probe);
  if (linearArray.get() != nullptr) {
    writeStringDataset(group, "probe_type", "LinearArray");
    writeLinearArray(group, linearArray);
    return;
  }

  // RcaArray ?
  std::shared_ptr<uff::RcaArray> rcaArray = std::dynamic_pointer_cast<uff::RcaArray>(probe);
  if (rcaArray.get() != nullptr) {
    writeStringDataset(group, "probe_type", "RcaArray");
    writeRcaArray(group, rcaArray);
    return;
  }
}

void Writer::writeProbeArray(H5::Group& group,
                             const std::vector<std::shared_ptr<uff::Probe>>& probes) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < probes.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    std::string probe_id = buf;
    H5::Group probe = group.createGroup(probe_id);
    writeProbe(probe, probes[i]);
  }
}

void Writer::writeReceiveSetup(H5::Group& group, const uff::ReceiveSetup& receiveSetup) {
  // "probe"
  std::string probeId =
      getIdFromPointer<uff::Probe>(m_dataset.channelData().probes(), receiveSetup.probe());
  writeStringDataset(group, "probe_id", probeId);

  // "time_offset"
  writeMetadataTypeDataset(group, "time_offset", receiveSetup.timeOffset());

  // "sampling_frequency"
  writeOptionalMetadataTypeDataset(group, "sampling_frequency", receiveSetup.samplingFrequency());

  // "sampling_type"
  writeIntegerDataset(group, "sampling_type", (int)receiveSetup.samplingType());

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

void Writer::writeRotation(H5::Group& group, const uff::Rotation& rotation) {
  writeMetadataTypeDataset(group, "x", rotation.x());
  writeMetadataTypeDataset(group, "y", rotation.y());
  writeMetadataTypeDataset(group, "z", rotation.z());
}

H5::DataSet Writer::writeStringDataset(H5::Group& group, const std::string& name,
                                       const std::string& value) {
  H5::StrType vlst(0, H5T_VARIABLE);
  H5::DataSpace ds_space(H5S_SCALAR);
  H5::DataSet dataset = group.createDataSet(name.c_str(), vlst, ds_space);
  dataset.write(value, vlst);
  return dataset;
}

H5::DataSet Writer::writeOptionalStringDataset(H5::Group& group, const std::string& name,
                                               const std::optional<std::string>& value) {
  H5::StrType vlst(0, H5T_VARIABLE);
  H5::DataSpace ds_space(H5S_SCALAR);
  H5::DataSet dataset = group.createDataSet(name.c_str(), vlst, ds_space);
  if (value.has_value())
    dataset.write(value.value(), vlst);
  else
    dataset.write(std::string("undefined"), vlst);
  return dataset;
}

void Writer::writeTimedEvent(H5::Group& group, const uff::TimedEvent& timedEvent) {
  // "event"
  std::string eventId =
      getIdFromPointer<uff::Event>(m_dataset.channelData().uniqueEvents(), timedEvent.evenement());
  writeStringDataset(group, "event_id", eventId);

  // "time_offset"
  writeMetadataTypeDataset(group, "time_offset", timedEvent.timeOffset());
}

void Writer::writeTimedEventArray(H5::Group& group,
                                  const std::vector<uff::TimedEvent>& timedEvents) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < timedEvents.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    std::string timedEvent_id = buf;
    H5::Group timedEvent = group.createGroup(timedEvent_id);
    writeTimedEvent(timedEvent, timedEvents[i]);
  }
}

void Writer::writeTransform(H5::Group& group, const uff::Transform& transform) {
  // "rotation"
  H5::Group rotation = group.createGroup("rotation");
  writeRotation(rotation, transform.rotation());

  // "translation"
  H5::Group translation = group.createGroup("translation");
  writeTranslation(translation, transform.translation());
}

void Writer::writeTranslation(H5::Group& group, const uff::Translation& translation) {
  writeMetadataTypeDataset(group, "x", translation.x());
  writeMetadataTypeDataset(group, "y", translation.y());
  writeMetadataTypeDataset(group, "z", translation.z());
}

void Writer::writeTransmitSetup(H5::Group& group, const uff::TransmitSetup& transmitSetup) {
  // "probe"
  const std::string probeId =
      getIdFromPointer<uff::Probe>(m_dataset.channelData().probes(), transmitSetup.probe());
  writeStringDataset(group, "probe_id", probeId);

  // "transmit_wave"
  H5::Group transmitWave = group.createGroup("transmit_wave");
  writeTransmitWave(transmitWave, transmitSetup.getTransmitWave());

  // "channel_mapping"
  writeIntegerArrayDataset(group, "channel_mapping", transmitSetup.channelMapping(), {});
}

void Writer::writeTransmitWave(H5::Group& group, const uff::TransmitWave& transmitWave) {
  // "wave"
  const std::string waveId =
      getIdFromPointer<uff::Wave>(m_dataset.channelData().uniqueWaves(), transmitWave.wave());
  writeStringDataset(group, "wave_id", waveId);

  // "time_offset"
  writeMetadataTypeDataset(group, "time_offset", transmitWave.timeOffset());

  // "weight"
  writeMetadataTypeDataset(group, "weight", transmitWave.weight());
}

void Writer::writeVersion(H5::Group& group, const uff::Version& version) {
  writeIntegerDataset(group, "major", version.major());
  writeIntegerDataset(group, "minor", version.minor());
  writeIntegerDataset(group, "patch", version.patch());
}

void Writer::writeWave(H5::Group& group, const std::shared_ptr<uff::Wave>& wave) {
  // write "origin"
  H5::Group origin = group.createGroup("origin");
  writeTransform(origin, wave->origin());

  // write "wave_type"
  writeIntegerDataset(group, "wave_type", (int)wave->waveType());

  // write "aperture"
  H5::Group aperture = group.createGroup("aperture");
  writeAperture(aperture, wave->aperture());

  // write "excitation"
  H5::Group excitation = group.createGroup("excitation");
  writeExcitation(excitation, wave->excitation());
}

void Writer::writeWaveArray(H5::Group& group,
                            const std::vector<std::shared_ptr<uff::Wave>>& waves) {
  char buf[9];
  snprintf(buf, sizeof buf, "%08d", 0);
  for (uint32_t i = 0; i < waves.size(); i++) {
    snprintf(buf, sizeof buf, "%08d", i + 1);
    std::string wave_id = buf;
    H5::Group wave = group.createGroup(wave_id);
    writeWave(wave, waves[i]);
  }
}

}  // namespace uff

#endif  // WITH_HDF5
