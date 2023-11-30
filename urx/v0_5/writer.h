/*!
 * Copyright Moduleus
 * \file urx/writer.h
 * \brief
 */

#ifdef WITH_HDF5
#ifndef URX_WRITER_H
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include <urx/v0_5/aperture.h>
#include <urx/v0_5/channel_data.h>
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
#include <urx/v0_5/version.h>
#include <urx/v0_5/wave.h>

#define URX_WRITER_H

#include <H5Cpp.h>
#include <optional>
#include <urx/v0_5/dataset.h>
#include <urx/v0_5/object.h>

namespace urx::v0_5 {

/**
 * @brief The URX Writer class
 */
template <typename DataType>
class Writer : public urx::Object {
  URX_TYPE_MACRO(Writer, urx::Object);

 public:
  Writer() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  //urx::Dataset* dataset() { return m_dataset.get(); }
  void setDataset(std::shared_ptr<const urx::Dataset<DataType>> dataset) { m_dataset = dataset; }

  /* Set/Get the filename of the URX file. The 'fileName' must contain the file extension. */
  std::string fileName() const { return m_fileName; }
  void setFileName(const std::string& fileName) { m_fileName = fileName; }

  /**
     * @brief Write the content of the Dataset in a file.
     * setDataset() and setFileName() must have been called before calling this method.
     */
  void writeToFile();

 private:
  template <typename T>
  std::string getIdFromPointer(const std::vector<std::shared_ptr<T>>& vec, std::weak_ptr<T> wptr);

  void writeAperture(H5::Group& group, const urx::Aperture& aperture);
  void writeChannelData(H5::Group& group, const urx::ChannelData<DataType>& channelData);
  H5::DataSet writeMetadataTypeDataset(H5::Group& group, const std::string& name,
                                       MetadataType value);
  H5::DataSet writeOptionalMetadataTypeDataset(H5::Group& group, const std::string& name,
                                               const std::optional<MetadataType>& value);

  void writeElementArray(H5::Group& group, const std::vector<urx::Element>& elements);
  void writeElement(H5::Group& group, const urx::Element& element);

  void writeEvent(H5::Group& group, const std::shared_ptr<urx::Event>& event);
  void writeEventArray(H5::Group& group, const std::vector<std::shared_ptr<urx::Event>>& events);

  void writeExcitation(H5::Group& group, const urx::Excitation& excitation);

  H5::DataSet writeDataTypeArrayDataset(H5::Group& group, const std::string& name,
                                        const std::vector<DataType>& values,
                                        const std::vector<size_t>& dimensions);

  H5::DataSet writeMetadataTypeArrayDataset(H5::Group& group, const std::string& name,
                                            const std::vector<MetadataType>& values,
                                            const std::vector<size_t>& dimensions);

  H5::DataSet writeIntegerArrayDataset(H5::Group& group, const std::string& name,
                                       const std::vector<int>& values,
                                       const std::vector<size_t>& dimensions);
  H5::DataSet writeIntegerDataset(H5::Group& group, const std::string& name, int value);

  void writeLinearArray(H5::Group& group, const std::shared_ptr<urx::LinearArray>& linearArray);
  void writeMatrixArray(H5::Group& group, const std::shared_ptr<urx::MatrixArray>& matrixArray);
  void writeRcaArray(H5::Group& group, const std::shared_ptr<urx::RcaArray>& rcaArray);

  void writeProbe(H5::Group& group, const std::shared_ptr<urx::Probe>& probe);
  void writeProbeArray(H5::Group& group, const std::vector<std::shared_ptr<urx::Probe>>& probes);

  void writeReceiveSetup(H5::Group& group, const urx::ReceiveSetup& receiveSetup);
  void writeRotation(H5::Group& group, const urx::Rotation& rotation);
  H5::DataSet writeStringDataset(H5::Group& group, const std::string& name,
                                 const std::string& value);
  H5::DataSet writeOptionalStringDataset(H5::Group& group, const std::string& name,
                                         const std::optional<std::string>& value);

  void writeTimedEvent(H5::Group& group, const urx::TimedEvent& timedEvent);
  void writeTimedEventArray(H5::Group& group, const std::vector<urx::TimedEvent>& timedEvents);

  void writeTransform(H5::Group& group, const urx::Transform& transform);
  void writeTranslation(H5::Group& group, const urx::Translation& translation);
  void writeTransmitSetup(H5::Group& group, const urx::TransmitSetup& transmitSetup);
  void writeTransmitWave(H5::Group& group, const urx::TransmitWave& transmitWave);
  void writeVersion(H5::Group& group, const urx::Version& version);

  void writeWave(H5::Group& group, const std::shared_ptr<urx::Wave>& wave);
  void writeWaveArray(H5::Group& group, const std::vector<std::shared_ptr<urx::Wave>>& waves);

  std::string m_fileName;
  std::shared_ptr<const urx::Dataset<DataType>> m_dataset;
};

}  // namespace urx::v0_5

#endif  // URX_WRITER_H
#endif  // WITH_HDF5
