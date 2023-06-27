#pragma once

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
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
 * @brief The UFF reader class
 * TODO: setFileName(), update(), 
 * TODO: only read metadata first, then read RF channel data next and only on-demand
 */
template <typename DataType>
class Reader {
 public:
  Reader() = default;

  std::shared_ptr<uff::Dataset<DataType>> dataset() { return _dataset; }
  std::shared_ptr<const uff::Dataset<DataType>> dataset() const { return _dataset; }

  /* Set/Get the filename of the UFF file. The 'filename' must contain the file extension. */
  std::string filename() const { return _filename; }
  void setFileName(const std::string& filename) { _filename = filename; }

  bool skipData() const { return _skip_data; }
  void setSkipData(bool skip) { _skip_data = skip; }

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

  //   static uff::Point2D readOptionnalPosition2D(const H5::Group& group);
  //   static uff::Point2D readOptionnalPosition3D(const H5::Group& group);

  //   static uff::Point2D readPosition2D(const H5::Group& group);
  //   static uff::Point3D readPosition3D(const H5::Group& group);

  static std::optional<uff::Point3D<MetadataType>> readElement(const H5::Group& group);
  static std::vector<std::optional<uff::Point3D<MetadataType>>> readElementArray(const H5::Group& group);

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
                                       const H5::PredType& targetType = H5DataType,
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

  static uff::Point3D<MetadataType> readRotation(const H5::Group& group);

  static uff::TimedEvent readTimedEvent(
      const H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents);
  static std::vector<uff::TimedEvent> readTimedEventArray(
      const H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents);

  static uff::Transform readTransform(const H5::Group& group);

  static uff::Point3D<MetadataType> readTranslation(const H5::Group& group);

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
  std::string _filename;
  bool _skip_data = false;

  // dataset
  std::shared_ptr<uff::Dataset<DataType>> _dataset;
};

}  // namespace uff