/*!
 * Copyright Moduleus
 * \file uff/reader.h
 * \brief
 */

#ifdef WITH_HDF5
#ifndef UFF_READER_H
#define UFF_READER_H

// UFF
#include "uff/dataset.h"
#include "uff/object.h"

#include <H5Cpp.h>

#include <optional>

namespace uff {

/**
 * @brief The UFF reader class
 * TODO: setFileName(), update(), 
 * TODO: only read metadata first, then read RF channel data next and only on-demand
 */
class Reader : public uff::Object {
  UFF_TYPE_MACRO(Reader, uff::Object);

 public:
  Reader() {}

  void printSelf(std::ostream& os, std::string indent) const override;

  std::shared_ptr<uff::Dataset> dataset() { return m_dataset; }

  /* Set/Get the filename of the UFF file. The 'fileName' must contain the file extension. */
  std::string fileName() const { return m_fileName; }
  void setFileName(const std::string& fileName) { m_fileName = fileName; }

  // No H5Exception is catched
  void updateMetadata();

 protected:
  // Read basic types
  MetadataType readMetadataTypeDataset(const H5::Group& group, const std::string& name);
  std::optional<MetadataType> readOptionalMetadataTypeDataset(const H5::Group& group,
                                                              const std::string& name);
  void readIntegerArrayDataset(const H5::Group& group, const std::string& name,
                               std::vector<int>& values, std::vector<size_t>& dimensions);
  int readIntegerDataset(const H5::Group& group, const std::string& name);
  std::string readStringDataset(const H5::Group& group, const std::string& name);
  std::optional<std::string> readOptionalStringDataset(const H5::Group& group,
                                                       const std::string& name);

  // Read groups
  uff::Aperture readAperture(const H5::Group& group);

  void readChannelData(const H5::Group& group);

  uff::Element readElement(const H5::Group& group);
  std::vector<uff::Element> readElementArray(const H5::Group& group);

  std::shared_ptr<uff::Event> readEvent(const H5::Group& group);
  std::vector<std::shared_ptr<uff::Event>> readEventArray(const H5::Group& group);

  uff::Excitation readExcitation(const H5::Group& group);

  void readDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                std::vector<DataType>& values, std::vector<size_t>& dimensions);
  void readMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                    std::vector<MetadataType>& values,
                                    std::vector<size_t>& dimensions);

  std::shared_ptr<uff::LinearArray> readLinearArray(const H5::Group& group);
  std::shared_ptr<uff::MatrixArray> readMatrixArray(const H5::Group& group);
  std::shared_ptr<uff::RcaArray> readRcaArray(const H5::Group& group);

  std::shared_ptr<uff::Probe> readProbe(const H5::Group& group);
  std::vector<std::shared_ptr<uff::Probe>> readProbeArray(const H5::Group& group);

  uff::ReceiveSetup readReceiveSetup(const H5::Group& group);

  uff::Rotation readRotation(const H5::Group& group);

  uff::TimedEvent readTimedEvent(const H5::Group& group);
  std::vector<uff::TimedEvent> readTimedEventArray(const H5::Group& group);

  uff::Transform readTransform(const H5::Group& group);

  uff::Translation readTranslation(const H5::Group& group);

  uff::TransmitSetup readTransmitSetup(const H5::Group& group);

  uff::TransmitWave readTransmitWave(const H5::Group& group);

  void readVersion(const H5::Group& group);

  std::shared_ptr<uff::Wave> readWave(const H5::Group& group);
  std::vector<std::shared_ptr<uff::Wave>> readWaveArray(const H5::Group& group);

 private:
  // name of the file to read
  std::string m_fileName;

  // dataset
  std::shared_ptr<uff::Dataset> m_dataset;
};

}  // namespace uff

#endif  // UFF_READER_H
#endif  // WITH_HDF5