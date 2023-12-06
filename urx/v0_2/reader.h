// Copyright Moduleus

#ifdef WITH_HDF5
#ifndef URX_READER_H
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/v0_2/aperture.h>
#include <urx/v0_2/element.h>
#include <urx/v0_2/event.h>
#include <urx/v0_2/excitation.h>
#include <urx/v0_2/linear_array.h>
#include <urx/v0_2/matrix_array.h>
#include <urx/v0_2/probe.h>
#include <urx/v0_2/rca_array.h>
#include <urx/v0_2/receive_setup.h>
#include <urx/v0_2/rotation.h>
#include <urx/v0_2/timed_event.h>
#include <urx/v0_2/transform.h>
#include <urx/v0_2/translation.h>
#include <urx/v0_2/transmit_setup.h>
#include <urx/v0_2/transmit_wave.h>
#include <urx/v0_2/urx.h>
#include <urx/v0_2/wave.h>

#define URX_READER_H

#include <optional>

#include <H5Cpp.h>

#include <urx/v0_2/dataset.h>
#include <urx/v0_2/object.h>

namespace urx::v0_2 {

/**
 * @brief The URX reader class
 * TODO: setFileName(), update(), 
 * TODO: only read metadata first, then read RF channel data next and only on-demand
 */
template <typename DataType>
class Reader : public Object {
  URX_TYPE_MACRO(Reader, Object);

 public:
  Reader() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::shared_ptr<Dataset<DataType>> dataset() { return m_dataset; }
  std::shared_ptr<const Dataset<DataType>> dataset() const { return m_dataset; }

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
  Aperture readAperture(const H5::Group& group);

  void readChannelData(const H5::Group& group);

  Element readElement(const H5::Group& group);
  std::vector<Element> readElementArray(const H5::Group& group);

  std::shared_ptr<Event> readEvent(const H5::Group& group);
  std::vector<std::shared_ptr<Event>> readEventArray(const H5::Group& group);

  Excitation readExcitation(const H5::Group& group);

  void readDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                std::vector<DataType>& values, std::vector<size_t>& dimensions);
  void readMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                    std::vector<MetadataType>& values,
                                    std::vector<size_t>& dimensions);

  std::shared_ptr<LinearArray> readLinearArray(const H5::Group& group);
  std::shared_ptr<MatrixArray> readMatrixArray(const H5::Group& group);
  std::shared_ptr<RcaArray> readRcaArray(const H5::Group& group);

  std::shared_ptr<Probe> readProbe(const H5::Group& group);
  std::vector<std::shared_ptr<Probe>> readProbeArray(const H5::Group& group);

  ReceiveSetup readReceiveSetup(const H5::Group& group);

  Rotation readRotation(const H5::Group& group);

  TimedEvent readTimedEvent(const H5::Group& group);
  std::vector<TimedEvent> readTimedEventArray(const H5::Group& group);

  Transform readTransform(const H5::Group& group);

  Translation readTranslation(const H5::Group& group);

  TransmitSetup readTransmitSetup(const H5::Group& group);

  TransmitWave readTransmitWave(const H5::Group& group);

  void readVersion(const H5::Group& group);

  std::shared_ptr<Wave> readWave(const H5::Group& group);
  std::vector<std::shared_ptr<Wave>> readWaveArray(const H5::Group& group);

 private:
  // name of the file to read
  std::string m_fileName;
  bool m_skipChannelDataData = false;

  // dataset
  std::shared_ptr<Dataset<DataType>> m_dataset;
};

}  // namespace urx::v0_2

#endif  // URX_READER_H
#endif  // WITH_HDF5