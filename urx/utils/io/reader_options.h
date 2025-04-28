#pragma once

namespace urx::utils::io {

enum class RawDataLoadPolicy {
  // RawData from file is loaded in memory when read method is called
  FULL,
  // RawData from file is never stored in memory.
  STREAM
};

class ReaderOptions {
 public:
  ReaderOptions() = default;
  ReaderOptions(RawDataLoadPolicy raw_data_load_policy)
      : _raw_data_load_policy(raw_data_load_policy) {}

  RawDataLoadPolicy getRawDataLoadPolicy() const { return _raw_data_load_policy; }
  void setRawDataLoadPolicy(RawDataLoadPolicy value) { _raw_data_load_policy = value; }

 private:
  RawDataLoadPolicy _raw_data_load_policy = RawDataLoadPolicy::FULL;
};

}  // namespace urx::utils::io
