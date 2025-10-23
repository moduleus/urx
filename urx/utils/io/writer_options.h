#pragma once

namespace urx::utils::io {

class WriterOptions {
 public:
  WriterOptions() = default;
  WriterOptions(bool chunk_group_data, bool clean_unusable_data = true, bool check_data = true)
      : _chunk_group_data(chunk_group_data),
        _clean_unusable_data(clean_unusable_data),
        _check_data(check_data) {}

  bool getChunkGroupData() const { return _chunk_group_data; }
  void setChunkGroupData(bool value) { _chunk_group_data = value; }

  bool getCleanUnusableData() const { return _clean_unusable_data; }
  void setCleanUnusableData(bool value) { _clean_unusable_data = value; }

  bool getCheckData() const { return _check_data; }
  void setCheckData(bool value) { _check_data = value; }

 private:
  bool _chunk_group_data = false;
  bool _clean_unusable_data = true;
  bool _check_data = true;
};

}  // namespace urx::utils::io
