/*!
 * Copyright Moduleus
 * \file uff/reader.h
 * \brief
 */

#ifdef WITH_HDF5
#ifndef UFF_READER_H
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "uff/aperture.h"
#include "uff/element.h"
#include "uff/event.h"
#include "uff/excitation.h"
#include "uff/linear_array.h"
#include "uff/matrix_array.h"
#include "uff/probe.h"
#include "uff/rca_array.h"
#include "uff/receive_setup.h"
#include "uff/rotation.h"
#include "uff/timed_event.h"
#include "uff/transform.h"
#include "uff/translation.h"
#include "uff/transmit_setup.h"
#include "uff/transmit_wave.h"
#include "uff/uff.h"
#include "uff/wave.h"

#define UFF_READER_H

#include <H5Cpp.h>
#include <optional>
#include "uff/dataset.h"
#include "uff/object.h"

namespace uff {

/**
 * @brief The UFF reader class
 * TODO: setFileName(), update(), 
 * TODO: only read metadata first, then read RF channel data next and only on-demand
 */
template <typename DataType>
class Reader : public uff::Object {
  UFF_TYPE_MACRO(Reader, uff::Object);

 public:
  Reader() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::shared_ptr<uff::Dataset<DataType>> dataset() { return m_dataset; }
  std::shared_ptr<const uff::Dataset<DataType>> dataset() const { return m_dataset; }

  /* Set/Get the filename of the UFF file. The 'fileName' must contain the file extension. */
  std::string fileName() const { return m_fileName; }
  void setFileName(const std::string& fileName) { m_fileName = fileName; }

  bool skipChannelDataData() const { return m_skipChannelDataData; }
  void setSkipChannelDataData(bool skip) { m_skipChannelDataData = skip; }

  // No H5Exception is catched
  void updateMetadata();

  static std::shared_ptr<uff::Dataset<DataType>> loadFile(std::string_view filename, bool castData,
                                                          bool skipChannelData);

 protected:
  // Read basic types
  static MetadataType readMetadataTypeDataset(const H5::Group& group, const std::string& name);
  static std::optional<MetadataType> readOptionalMetadataTypeDataset(const H5::Group& group,
                                                                     const std::string& name);
  static void readIntegerArrayDataset(const H5::Group& group, const std::string& name,
                                      std::vector<int>& values, std::vector<hsize_t>& dimensions);
  static int readIntegerDataset(const H5::Group& group, const std::string& name);
  static std::string readStringDataset(const H5::Group& group, const std::string& name);
  static std::optional<std::string> readOptionalStringDataset(const H5::Group& group,
                                                              const std::string& name);

  // Read groups
  static uff::Aperture readAperture(const H5::Group& group);

  static uff::ChannelData<DataType> readChannelData(const H5::Group& group, bool castData = false,
                                                    bool skipData = false);

  static uff::Element readElement(const H5::Group& group);
  static std::vector<uff::Element> readElementArray(const H5::Group& group);

  static std::shared_ptr<uff::Event> readEvent(
      const H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes,
      const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves);
  static std::vector<std::shared_ptr<uff::Event>> readEventArray(
      const H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes,
      const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves);

  static uff::Excitation readExcitation(const H5::Group& group);

  static void readDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                       std::vector<DataType>& values,
                                       std::vector<hsize_t>& dimensions,
                                       H5::PredType targetType = H5DataType,
                                       bool skipChannelData = false);
  static void readMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                           std::vector<MetadataType>& values,
                                           std::vector<hsize_t>& dimensions);

  static std::shared_ptr<uff::LinearArray> readLinearArray(const H5::Group& group);
  static std::shared_ptr<uff::MatrixArray> readMatrixArray(const H5::Group& group);
  static std::shared_ptr<uff::RcaArray> readRcaArray(const H5::Group& group);

  static std::shared_ptr<uff::Probe> readProbe(const H5::Group& group);
  static std::vector<std::shared_ptr<uff::Probe>> readProbeArray(const H5::Group& group);

  static uff::ReceiveSetup readReceiveSetup(const H5::Group& group,
                                            const std::vector<std::shared_ptr<uff::Probe>>& probes);

  static uff::Rotation readRotation(const H5::Group& group);

  static uff::TimedEvent readTimedEvent(
      const H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents);
  static std::vector<uff::TimedEvent> readTimedEventArray(
      const H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents);

  static uff::Transform readTransform(const H5::Group& group);

  static uff::Translation readTranslation(const H5::Group& group);

  static uff::TransmitSetup readTransmitSetup(
      const H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes,
      const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves);

  static uff::TransmitWave readTransmitWave(
      const H5::Group& group, const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves);

  static uff::Version readVersion(const H5::Group& group);

  static std::shared_ptr<uff::Wave> readWave(const H5::Group& group);
  static std::vector<std::shared_ptr<uff::Wave>> readWaveArray(const H5::Group& group);

 private:
  // name of the file to read
  std::string m_fileName;
  bool m_skipChannelDataData = false;

  // dataset
  std::shared_ptr<uff::Dataset<DataType>> m_dataset;
};

}  // namespace uff

#endif  // UFF_READER_H
#endif  // WITH_HDF5