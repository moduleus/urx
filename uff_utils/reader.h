#pragma once

#include <H5Cpp.h>
#include <uff/dataset.h>
#include <uff/excitation.h>
#include <uff/uff.h>
#include <uff_utils/io.h>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace uff {

/**
 * @brief The UFF reader class
 * TODO: setFilename(), update(), 
 * TODO: only read metadata first, then read RF channel data next and only on-demand
 */
class Reader {
 public:
  Reader() = default;

  std::shared_ptr<Dataset> dataset() { return _dataset; }
  std::shared_ptr<const Dataset> dataset() const { return _dataset; }

  /* Set/Get the filename of the UFF file. The 'filename' must contain the file extension. */
  std::string filename() const { return _filename; }
  void setFilename(const std::string& filename) { _filename = filename; }

  bool skipData() const { return _skip_data; }
  void setSkipData(bool skip) { _skip_data = skip; }

  // No H5Exception is catched
  void updateMetadata();

  static std::shared_ptr<Dataset> loadFile(std::string_view filename, bool castData,
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

  static Excitation readExcitation(const H5::Group& group);

  static void readDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                       std::vector<DataType>& values,
                                       std::vector<hsize_t>& dimensions,
                                       const H5::PredType& targetType = H5DataType,
                                       bool skipChannelData = false);
  static void readMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                           std::vector<MetadataType>& values,
                                           std::vector<hsize_t>& dimensions);

 private:
  // name of the file to read
  std::string _filename;
  bool _skip_data = false;

  // dataset
  std::shared_ptr<Dataset> _dataset;
};

}  // namespace uff