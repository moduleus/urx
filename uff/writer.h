/*!
 * Copyright Moduleus
 * \file uff/writer.h
 * \brief
 */

#ifdef WITH_HDF5
#ifndef UFF_WRITER_H
#define UFF_WRITER_H

// UFF
#include "uff/dataset.h"
#include "uff/object.h"

#include <H5Cpp.h>
#include <optional>

namespace uff {

/**
 * @brief The UFF Writer class
 */
class Writer : public uff::Object {
  UFF_TYPE_MACRO(Writer, uff::Object);

 public:
  Writer() {}

  void printSelf(std::ostream& os, std::string indent) const override;

  //uff::Dataset* dataset() { return m_dataset.get(); }
  void setDataset(std::shared_ptr<uff::Dataset> dataset) { m_dataset = dataset; }

  /* Set/Get the filename of the UFF file. The 'fileName' must contain the file extension. */
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

  void writeAperture(H5::Group& group, const uff::Aperture& aperture);
  void writeChannelData(H5::Group& group, const uff::ChannelData& channelData);
  H5::DataSet writeMetadataTypeDataset(H5::Group& group, const std::string& name,
                                       MetadataType value);
  H5::DataSet writeOptionalMetadataTypeDataset(H5::Group& group, const std::string& name,
                                               std::optional<MetadataType> value);

  void writeElementArray(H5::Group& group, const std::vector<uff::Element>& elements);
  void writeElement(H5::Group& group, const uff::Element& element);

  void writeEvent(H5::Group& group, const std::shared_ptr<uff::Event>& event);
  void writeEventArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& events);

  void writeExcitation(H5::Group& group, const uff::Excitation& excitation);

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

  void writeLinearArray(H5::Group& group, const std::shared_ptr<uff::LinearArray>& linearArray);
  void writeMatrixArray(H5::Group& group, const std::shared_ptr<uff::MatrixArray>& matrixArray);
  void writeRcaArray(H5::Group& group, const std::shared_ptr<uff::RcaArray>& rcaArray);

  void writeProbe(H5::Group& group, const std::shared_ptr<uff::Probe>& probe);
  void writeProbeArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes);

  void writeReceiveSetup(H5::Group& group, const uff::ReceiveSetup& receiveSetup);
  void writeRotation(H5::Group& group, const uff::Rotation& rotation);
  H5::DataSet writeStringDataset(H5::Group& group, const std::string& name,
                                 const std::string& value);
  H5::DataSet writeOptionalStringDataset(H5::Group& group, const std::string& name,
                                         const std::optional<std::string>& value);

  void writeTimedEvent(H5::Group& group, const uff::TimedEvent& timedEvent);
  void writeTimedEventArray(H5::Group& group, const std::vector<uff::TimedEvent>& timedEvents);

  void writeTransform(H5::Group& group, const uff::Transform& transform);
  void writeTranslation(H5::Group& group, const uff::Translation& translation);
  void writeTransmitSetup(H5::Group& group, const uff::TransmitSetup& transmitSetup);
  void writeTransmitWave(H5::Group& group, const uff::TransmitWave& transmitWave);
  void writeVersion(H5::Group& group, const uff::Version& version);

  void writeWave(H5::Group& group, const std::shared_ptr<uff::Wave>& wave);
  void writeWaveArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Wave>>& waves);

 private:
  std::string m_fileName;
  std::shared_ptr<uff::Dataset> m_dataset;
};

}  // namespace uff

#endif  // UFF_WRITER_H
#endif  // WITH_HDF5
