/*!
 * Copyright Moduleus
 * \file urx/reader.h
 * \brief
 */

#ifdef WITH_HDF5
#ifndef URX_READER_H
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include <urx/v0_5/aperture.h>
#include <urx/v0_5/element.h>
#include <urx/v0_5/event.h>
#include <urx/v0_5/excitation.h>
#include <urx/v0_5/linear_array.h>
#include <urx/v0_5/matrix_array.h>
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
#include <urx/v0_5/wave.h>

#define URX_READER_H

#include <optional>

#include <H5Cpp.h>

#include <urx/v0_5/dataset.h>
#include <urx/v0_5/object.h>

namespace urx::v0_5 {

/**
 * @brief The URX reader class
 * TODO: setFileName(), update(), 
 * TODO: only read metadata first, then read RF channel data next and only on-demand
 */
template <typename DataType>
class Reader : public urx::Object {
  URX_TYPE_MACRO(Reader, urx::Object);

 public:
  Reader() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::shared_ptr<urx::Dataset<DataType>> dataset() { return m_dataset; }
  std::shared_ptr<const urx::Dataset<DataType>> dataset() const { return m_dataset; }

  /* Set/Get the filename of the URX file. The 'fileName' must contain the file extension. */
  std::string fileName() const { return m_fileName; }
  void setFileName(const std::string& fileName) { m_fileName = fileName; }

  bool skipChannelDataData() const { return m_skipChannelDataData; }
  void setSkipChannelDataData(bool skip) { m_skipChannelDataData = skip; }

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
  urx::Aperture readAperture(const H5::Group& group);

  void readChannelData(const H5::Group& group);

  urx::Element readElement(const H5::Group& group);
  std::vector<urx::Element> readElementArray(const H5::Group& group);

  std::shared_ptr<urx::Event> readEvent(const H5::Group& group);
  std::vector<std::shared_ptr<urx::Event>> readEventArray(const H5::Group& group);

  urx::Excitation readExcitation(const H5::Group& group);

  void readDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                std::vector<DataType>& values, std::vector<size_t>& dimensions);
  void readMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                    std::vector<MetadataType>& values,
                                    std::vector<size_t>& dimensions);

  std::shared_ptr<urx::LinearArray> readLinearArray(const H5::Group& group);
  std::shared_ptr<urx::MatrixArray> readMatrixArray(const H5::Group& group);
  std::shared_ptr<urx::RcaArray> readRcaArray(const H5::Group& group);

  std::shared_ptr<urx::Probe> readProbe(const H5::Group& group);
  std::vector<std::shared_ptr<urx::Probe>> readProbeArray(const H5::Group& group);

  urx::ReceiveSetup readReceiveSetup(const H5::Group& group);

  urx::Rotation readRotation(const H5::Group& group);

  urx::TimedEvent readTimedEvent(const H5::Group& group);
  std::vector<urx::TimedEvent> readTimedEventArray(const H5::Group& group);

  urx::Transform readTransform(const H5::Group& group);

  urx::Translation readTranslation(const H5::Group& group);

  urx::TransmitSetup readTransmitSetup(const H5::Group& group);

  urx::TransmitWave readTransmitWave(const H5::Group& group);

  void readVersion(const H5::Group& group);

  std::shared_ptr<urx::Wave> readWave(const H5::Group& group);
  std::vector<std::shared_ptr<urx::Wave>> readWaveArray(const H5::Group& group);

 private:
  // name of the file to read
  std::string m_fileName;
  bool m_skipChannelDataData = false;

  // dataset
  std::shared_ptr<urx::Dataset<DataType>> m_dataset;
};

}  // namespace urx::v0_5

#endif  // URX_READER_H
#endif  // WITH_HDF5