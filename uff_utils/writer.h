#pragma once

#include <H5Cpp.h>
#include <uff/dataset.h>
#include <uff/uff.h>
#include <uff/vector.h>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace uff {

/**
 * @brief The UFF Writer class
 */
class Writer {
 public:
  Writer() = default;

  //Dataset* dataset() { return _dataset.get(); }
  inline void setDataset(const std::shared_ptr<const Dataset>& dataset) { _dataset = dataset; }

  /* Set/Get the filename of the UFF file. The 'filename' must contain the file extension. */
  inline std::string filename() const { return _filename; }
  inline void setFilename(const std::string& filename) { _filename = filename; }

  /**
     * @brief Write the content of the Dataset in a file.
     * setDataset() and setFileName() must have been called before calling this method.
     */
  void writeToFile();

 private:
  template <typename T>
  std::string getIdFromPointer(const std::vector<std::shared_ptr<T>>& vec,
                               const std::weak_ptr<T>& wptr);

  static void writeMetadataTypeDataset(const H5::Group& group, const std::string& name,
                                       double value);
  static void writeOptionalMetadataTypeDataset(const H5::Group& group, const std::string& name,
                                               const std::optional<double>& value);

  static void writePoint2D(const H5::Group& group, const Vector2D<double>& position);
  static void writePoint3D(const H5::Group& group, const Vector3D<double>& position);

  static void writeDataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                        const std::vector<DataType>& values,
                                        const std::vector<size_t>& dimensions);

  static void writeMetadataTypeArrayDataset(const H5::Group& group, const std::string& name,
                                            const std::vector<double>& values,
                                            const std::vector<size_t>& dimensions);

  static void writeIntegerArrayDataset(const H5::Group& group, const std::string& name,
                                       const std::vector<int>& values,
                                       const std::vector<size_t>& dimensions);
  static void writeIntegerDataset(const H5::Group& group, const std::string& name, int value);

  std::string _filename;
  std::shared_ptr<const Dataset> _dataset;
};

}  // namespace uff
