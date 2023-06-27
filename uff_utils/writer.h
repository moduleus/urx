#pragma once

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <uff/aperture.h>
#include <uff/channel_data.h>
#include <uff/dataset.h>
#include <uff/event.h>
#include <uff/excitation.h>
#include <uff/linear_array.h>
#include <uff/matrix_array.h>
#include <uff/point.h>
#include <uff/probe.h>
#include <uff/rca_array.h>
#include <uff/receive_setup.h>
#include <uff/timed_event.h>
#include <uff/transform.h>
#include <uff/transmit_setup.h>
#include <uff/transmit_wave.h>
#include <uff/uff.h>
#include <uff/version.h>
#include <uff/wave.h>

#include <uff_utils/io.h>

namespace uff {

/**
 * @brief The UFF Writer class
 */
template <typename DataType>
class Writer {
 public:
  Writer() = default;

  //uff::Dataset* dataset() { return _dataset.get(); }
  void setDataset(std::shared_ptr<const uff::Dataset<DataType>> dataset) { _dataset = dataset; }

  /* Set/Get the filename of the UFF file. The 'fileName' must contain the file extension. */
  std::string fileName() const { return _fileName; }
  void setFileName(const std::string& fileName) { _fileName = fileName; }

  /**
     * @brief Write the content of the Dataset in a file.
     * setDataset() and setFileName() must have been called before calling this method.
     */
  void writeToFile();

 private:
  template <typename T>
  std::string getIdFromPointer(const std::vector<std::shared_ptr<T>>& vec, std::weak_ptr<T> wptr);

  void writeAperture(H5::Group& group, const uff::Aperture& aperture);
  void writeChannelData(H5::Group& group, const uff::ChannelData<DataType>& channelData);
  void writeMetadataTypeDataset(H5::Group& group, const std::string& name, MetadataType value);
  void writeOptionalMetadataTypeDataset(H5::Group& group, const std::string& name,
                                        const std::optional<MetadataType>& value);

  void writeElementArray(H5::Group& group, const std::vector<std::optional<uff::Point3D<MetadataType>>>& elements);
  void writeElement(H5::Group& group, const std::optional<uff::Point3D<MetadataType>>& element);

  void writePoint2D(H5::Group& group, const uff::Point2D<MetadataType>& position);
  void writePoint3D(H5::Group& group, const uff::Point3D<MetadataType>& position);

  void writeEvent(H5::Group& group, const std::shared_ptr<uff::Event>& event);
  void writeEventArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& events);

  void writeExcitation(H5::Group& group, const uff::Excitation& excitation);

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

  void writeLinearArray(H5::Group& group, const std::shared_ptr<uff::LinearArray>& linearArray);
  void writeMatrixArray(H5::Group& group, const std::shared_ptr<uff::MatrixArray>& matrixArray);
  void writeRcaArray(H5::Group& group, const std::shared_ptr<uff::RcaArray>& rcaArray);

  void writeProbe(H5::Group& group, const std::shared_ptr<uff::Probe>& probe);
  void writeProbeArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes);

  void writeReceiveSetup(H5::Group& group, const uff::ReceiveSetup& receiveSetup);
  void writeRotation(H5::Group& group, const uff::Point3D<MetadataType>& rotation);
  void writeStringDataset(H5::Group& group, const std::string& name, const std::string& value);
  void writeOptionalStringDataset(H5::Group& group, const std::string& name,
                                  const std::optional<std::string>& value);

  void writeTimedEvent(H5::Group& group, const uff::TimedEvent& timedEvent);
  void writeTimedEventArray(H5::Group& group, const std::vector<uff::TimedEvent>& timedEvents);

  void writeTransform(H5::Group& group, const uff::Transform& transform);
  void writeTranslation(H5::Group& group, const uff::Point3D<MetadataType>& translation);
  void writeTransmitSetup(H5::Group& group, const uff::TransmitSetup& transmitSetup);
  void writeTransmitWave(H5::Group& group, const uff::TransmitWave& transmitWave);
  void writeVersion(H5::Group& group, const uff::Version& version);

  void writeWave(H5::Group& group, const std::shared_ptr<uff::Wave>& wave);
  void writeWaveArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Wave>>& waves);

  std::string _fileName;
  std::shared_ptr<const uff::Dataset<DataType>> _dataset;
};

}  // namespace uff
