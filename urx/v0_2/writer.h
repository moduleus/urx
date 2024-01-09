// Copyright Moduleus

#ifdef WITH_HDF5
#ifndef URX_WRITER_H
#define URX_WRITER_H

#include <iosfwd>
#include <memory>
#include <string>
#include <utility>

#include <urx/v0_2/dataset.h>
#include <urx/v0_2/object.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief The URX Writer class
 */
template <typename DataType>
class Writer : public Object {
  URX_TYPE_MACRO(Writer, Object);

 public:
  Writer();
  ~Writer() override;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  //Dataset* dataset() { return m_dataset.get(); }
  void setDataset(std::shared_ptr<const Dataset<DataType>> dataset);

  /* Set/Get the filename of the URX file. The 'fileName' must contain the file extension. */
  std::string fileName() const { return m_fileName; }
  void setFileName(const std::string& fileName) { m_fileName = fileName; }

  /**
     * @brief Write the content of the Dataset in a file.
     * setDataset() and setFileName() must have been called before calling this method.
     */
  void writeToFile();

 private:
  class WriterImpl;
  std::unique_ptr<WriterImpl> _impl;

  std::string m_fileName;
};

}  // namespace urx::v0_2

#endif  // URX_WRITER_H
#endif  // WITH_HDF5
