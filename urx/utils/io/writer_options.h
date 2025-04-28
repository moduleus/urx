#pragma once

namespace urx::utils::io {

class WriterOptions {
 public:
  WriterOptions() = default;
  WriterOptions(bool chunk_group_data) : _chunk_group_data(chunk_group_data) {}

  bool getChunkGroupData() const { return _chunk_group_data; }
  void setChunkGroupData(bool value) { _chunk_group_data = value; }

 private:
  bool _chunk_group_data = false;
};

}  // namespace urx::utils::io
