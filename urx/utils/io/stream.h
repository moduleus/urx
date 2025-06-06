#pragma once

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include <H5Cpp.h>

#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/utils/export.h>
#include <urx/utils/io/reader_impl.h>
#include <urx/utils/io/reader_options.h>
#include <urx/utils/io/writer_impl.h>

namespace urx::utils::io {

class GroupDataStream {
 public:
  URX_UTILS_EXPORT GroupDataStream(std::vector<urx::GroupData>* group_data, H5::Group& h5_group,
                                   urx::utils::io::WriterDataset* writer);

  URX_UTILS_EXPORT void append(const std::shared_ptr<RawData>& raw_data, double sequence_timestamp,
                               const std::vector<double>& event_timestamp);

  URX_UTILS_EXPORT urx::GroupData& getGroupData();

 private:
  std::vector<urx::GroupData>* _group_data;
  size_t _group_data_idx;
  H5::Group _h5_group_data;
  urx::utils::io::WriterDataset* _writer;
  H5::DataSet _h5_sequence_timestamps;
  H5::Group _h5_event_timestamps;
  H5::DataSet _h5_raw_data;

  template <typename T>
  void _extendVector(H5::Group& group, H5::DataSet& dataset, std::vector<T>& vector,
                     const T& value);

  void _extend(H5::Group& group, H5::DataSet& dataset, const std::shared_ptr<RawData>& value);
};

class Stream {
 public:
  URX_UTILS_EXPORT Stream(const std::string& filename, std::shared_ptr<Dataset> dataset);

  URX_UTILS_EXPORT std::shared_ptr<Dataset> getDataset() const;

  URX_UTILS_EXPORT void saveToFile();
  URX_UTILS_EXPORT void loadFromFile();

  URX_UTILS_EXPORT RawDataLoadPolicy getRawDataLoadPolicy() const;
  URX_UTILS_EXPORT void setRawDataLoadPolicy(RawDataLoadPolicy value);

  URX_UTILS_EXPORT bool getChunkGroupData() const;
  URX_UTILS_EXPORT void setChunkGroupData(bool value);

  URX_UTILS_EXPORT GroupDataStream createGroupData(const std::shared_ptr<Group>& group,
                                                   DoubleNan timestamp);

  URX_UTILS_EXPORT void readRawData(size_t group_data,
                                    const std::shared_ptr<urx::RawData>& raw_data,
                                    size_t sequence_urx_raw_data, size_t sequence_h5_raw_data,
                                    size_t count);

 private:
  urx::utils::io::ReaderDataset _reader;
  urx::utils::io::WriterDataset _writer;

  std::shared_ptr<Dataset> _dataset;
  H5::H5File _file;
};

}  // namespace urx::utils::io
