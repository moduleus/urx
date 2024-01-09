// Copyright Moduleus

#ifdef WITH_HDF5
#ifndef URX_READER_H

#define URX_READER_H

#include <iosfwd>
#include <memory>
#include <string>
#include <utility>

#include <urx/v0_2/dataset.h>
#include <urx/v0_2/object.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief The URX reader class
 * TODO: setFileName(), update(), 
 * TODO: only read metadata first, then read RF channel data next and only on-demand
 */
template <typename DataType>
class Reader : public Object {
  URX_TYPE_MACRO(Reader, Object);

 public:
  Reader();
  ~Reader() override;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  std::shared_ptr<Dataset<DataType>> dataset();
  std::shared_ptr<const Dataset<DataType>> dataset() const;

  /* Set/Get the filename of the URX file. The 'fileName' must contain the file extension. */
  std::string fileName() const { return m_fileName; }
  void setFileName(const std::string& fileName) { m_fileName = fileName; }

  bool skipChannelDataData() const;
  void setSkipChannelDataData(bool skip);

  // No H5Exception is catched
  void updateMetadata();

 private:
  class ReaderImpl;
  std::unique_ptr<ReaderImpl> _impl;

  // name of the file to read
  std::string m_fileName;
};

}  // namespace urx::v0_2

#endif  // URX_READER_H
#endif  // WITH_HDF5