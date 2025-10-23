#pragma once

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/utils/export.h>
#include <urx/utils/io/reader_options.h>
#include <urx/utils/io/writer_options.h>

namespace urx::utils::io {

class GroupDataStream {
 public:
  URX_UTILS_EXPORT GroupDataStream();
  GroupDataStream(const GroupDataStream& other) = delete;
  GroupDataStream(GroupDataStream&& other) noexcept = default;
  GroupDataStream& operator=(GroupDataStream const& other) = delete;
  GroupDataStream& operator=(GroupDataStream&& other) noexcept = default;
  URX_UTILS_EXPORT ~GroupDataStream();

  URX_UTILS_EXPORT void append(const std::shared_ptr<RawData>& raw_data, double sequence_timestamp,
                               const std::vector<double>& event_timestamp);

  URX_UTILS_EXPORT urx::GroupData& getGroupData();

 private:
  struct GroupDataStreamHdf5Fields;

  std::unique_ptr<GroupDataStreamHdf5Fields> _impl;
  std::vector<urx::GroupData>* _group_data;
  size_t _group_data_idx;

  friend class Stream;
};

class Stream {
 public:
  URX_UTILS_EXPORT Stream(const std::string& filename, std::shared_ptr<Dataset> dataset);
  URX_UTILS_EXPORT ~Stream();

  URX_UTILS_EXPORT std::shared_ptr<Dataset> getDataset() const;

  URX_UTILS_EXPORT void saveToFile();
  URX_UTILS_EXPORT void loadFromFile();

  [[deprecated("Use readerOptions() instead.")]] URX_UTILS_EXPORT RawDataLoadPolicy
  getRawDataLoadPolicy() const;
  [[deprecated("Use readerOptions() instead.")]] URX_UTILS_EXPORT void setRawDataLoadPolicy(
      RawDataLoadPolicy value);

  [[deprecated("Use writerOptions() instead.")]] URX_UTILS_EXPORT bool getChunkGroupData() const;
  [[deprecated("Use writerOptions() instead.")]] URX_UTILS_EXPORT void setChunkGroupData(
      bool value);

  URX_UTILS_EXPORT const ReaderOptions& readerOptions() const;
  URX_UTILS_EXPORT ReaderOptions& readerOptions();

  URX_UTILS_EXPORT const WriterOptions& writerOptions() const;
  URX_UTILS_EXPORT WriterOptions& writerOptions();

  URX_UTILS_EXPORT GroupDataStream createGroupData(const std::shared_ptr<Group>& group,
                                                   DoubleNan timestamp);

  URX_UTILS_EXPORT void readRawData(size_t group_data,
                                    const std::shared_ptr<urx::RawData>& raw_data,
                                    size_t sequence_urx_raw_data, size_t sequence_h5_raw_data,
                                    size_t count);
  URX_UTILS_EXPORT void readRawData(size_t group_data, void* raw_data, DataType data_type,
                                    SamplingType sampling_type, size_t sequence_urx_raw_data,
                                    size_t sequence_h5_raw_data, size_t count);

 private:
  struct StreamHdf5Fields;

  std::unique_ptr<StreamHdf5Fields> _impl;

  std::shared_ptr<Dataset> _dataset;
};

}  // namespace urx::utils::io
