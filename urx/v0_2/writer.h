/*!
 * Copyright Moduleus
 * \file urx/writer.h
 * \brief
 */

#ifdef WITH_HDF5
#ifndef URX_WRITER_H
#define URX_WRITER_H

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <H5Cpp.h>

#include <urx/v0_2/aperture.h>
#include <urx/v0_2/channel_data.h>
#include <urx/v0_2/dataset.h>
#include <urx/v0_2/element.h>
#include <urx/v0_2/event.h>
#include <urx/v0_2/excitation.h>
#include <urx/v0_2/linear_array.h>
#include <urx/v0_2/matrix_array.h>
#include <urx/v0_2/object.h>
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
#include <urx/v0_2/version.h>
#include <urx/v0_2/wave.h>

namespace urx::v0_2 {

/**
 * @brief The URX Writer class
 */
template <typename DataType>
class Writer : public Object {
  URX_TYPE_MACRO(Writer, Object);

 public:
  Writer() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  //Dataset* dataset() { return m_dataset.get(); }
  void setDataset(std::shared_ptr<const Dataset<DataType>> dataset) { m_dataset = dataset; }

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

  void writeAperture(H5::Group& group, const Aperture& aperture);
  void writeChannelData(H5::Group& group, const ChannelData<DataType>& channelData);
  void writeMetadataTypeDataset(H5::Group& group, const std::string& name, MetadataType value);
  void writeOptionalMetadataTypeDataset(H5::Group& group, const std::string& name,
                                        const std::optional<MetadataType>& value);

  void writeElementArray(H5::Group& group, const std::vector<Element>& elements);
  void writeElement(H5::Group& group, const Element& element);

  void writeEvent(H5::Group& group, const std::shared_ptr<Event>& event);
  void writeEventArray(H5::Group& group, const std::vector<std::shared_ptr<Event>>& events);

  void writeExcitation(H5::Group& group, const Excitation& excitation);

  void writeDataTypeArrayDataset(H5::Group& group, const std::string& name,
                                 const std::vector<DataType>& values,
                                 const std::vector<size_t>& dimensions);

  void writeMetadataTypeArrayDataset(H5::Group& group, const std::string& name,
                                     const std::vector<MetadataType>& values,
                                     const std::vector<size_t>& dimensions);

  void writeIntegerArrayDataset(H5::Group& group, const std::string& name,
                                const std::vector<int>& values,
                                const std::vector<size_t>& dimensions);
  void writeIntegerDataset(H5::Group& group, const std::string& name, int value);

  void writeLinearArray(H5::Group& group, const std::shared_ptr<LinearArray>& linearArray);
  void writeMatrixArray(H5::Group& group, const std::shared_ptr<MatrixArray>& matrixArray);
  void writeRcaArray(H5::Group& group, const std::shared_ptr<RcaArray>& rcaArray);

  void writeProbe(H5::Group& group, const std::shared_ptr<Probe>& probe);
  void writeProbeArray(H5::Group& group, const std::vector<std::shared_ptr<Probe>>& probes);

  void writeReceiveSetup(H5::Group& group, const ReceiveSetup& receiveSetup);
  void writeRotation(H5::Group& group, const Rotation& rotation);
  void writeStringDataset(H5::Group& group, const std::string& name, const std::string& value);
  void writeOptionalStringDataset(H5::Group& group, const std::string& name,
                                  const std::optional<std::string>& value);

  void writeTimedEvent(H5::Group& group, const TimedEvent& timedEvent);
  void writeTimedEventArray(H5::Group& group, const std::vector<TimedEvent>& timedEvents);

  void writeTransform(H5::Group& group, const Transform& transform);
  void writeTranslation(H5::Group& group, const Translation& translation);
  void writeTransmitSetup(H5::Group& group, const TransmitSetup& transmitSetup);
  void writeTransmitWave(H5::Group& group, const TransmitWave& transmitWave);
  void writeVersion(H5::Group& group, const Version& version);

  void writeWave(H5::Group& group, const std::shared_ptr<Wave>& wave);
  void writeWaveArray(H5::Group& group, const std::vector<std::shared_ptr<Wave>>& waves);

  std::string m_fileName;
  std::shared_ptr<const Dataset<DataType>> m_dataset;
};

}  // namespace urx::v0_2

#endif  // URX_WRITER_H
#endif  // WITH_HDF5
