#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <H5Cpp.h>

#include <urx/dataset.h>
#include <urx/excitation.h>
#include <urx/urx.h>
#include <urx_utils/io.h>

namespace urx {

/**
 * @brief The URX reader class
 * TODO: setFilename(), update(), 
 * TODO: only read metadata first, then read RF channel data next and only on-demand
 */
class Reader {
 public:
  Reader() = default;

  std::shared_ptr<Dataset> dataset() { return _dataset; }
  std::shared_ptr<const Dataset> dataset() const { return _dataset; }

  /* Set/Get the filename of the URX file. The 'filename' must contain the file extension. */
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
  static double readMetadataTypeDataset(const H5::Group& group, const std::string& name);
  static std::optional<double> readOptionalMetadataTypeDataset(const H5::Group& group,
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
                                           std::vector<double>& values,
                                           std::vector<hsize_t>& dimensions);

 private:
  // name of the file to read
  std::string _filename;
  bool _skip_data = false;

  // dataset
  std::shared_ptr<Dataset> _dataset;
};

}  // namespace urx