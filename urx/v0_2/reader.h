// Copyright Moduleus

#ifdef WITH_HDF5
#ifndef URX_READER_H

#define URX_READER_H

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <urx/v0_2/aperture.h>
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
#include <urx/v0_2/wave.h>

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