#pragma once

#include <H5Cpp.h>
#include <uff/dataset.h>
#include <uff/point.h>
#include <uff/uff.h>
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
  void setDataset(std::shared_ptr<const Dataset> dataset) { _dataset = dataset; }

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

  void writeMetadataTypeDataset(H5::Group& group, const std::string& name, MetadataType value);
  void writeOptionalMetadataTypeDataset(H5::Group& group, const std::string& name,
                                        const std::optional<MetadataType>& value);

  void writePoint2D(H5::Group& group, const Point2D<MetadataType>& position);
  void writePoint3D(H5::Group& group, const Point3D<MetadataType>& position);

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

  std::string _fileName;
  std::shared_ptr<const Dataset> _dataset;
};

}  // namespace uff
