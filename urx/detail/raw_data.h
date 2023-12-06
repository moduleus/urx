#pragma once

#include <memory>
#include <vector>

#include <urx/group.h>

namespace urx {

class RawData {
 public:
  virtual const void* getBuffer() const = 0;
  virtual void* getBuffer() = 0;
  virtual size_t getSize() const = 0;

 protected:
  ~RawData() = default;
};

template <typename T>
class RawDataVector final : public RawData {
 public:
  explicit RawDataVector(std::vector<T>&& vector) : _vector(std::move(vector)) {}

  const void* getBuffer() const override { return _vector.data(); }
  void* getBuffer() override { return _vector.data(); }
  size_t getSize() const override { return _vector.size(); }

 private:
  std::vector<T> _vector;
};

template <typename T>
class RawDataNoInit final : public RawData {
 public:
  explicit RawDataNoInit(size_t size)
      : _buffer(std::make_unique_for_overwrite<T[]>(size)), _size(size) {}

  const void* getBuffer() const override { return _buffer.get(); }
  void* getBuffer() override { return _buffer.get(); }
  size_t getSize() const override { return _size; }

 private:
  std::unique_ptr<T[]> _buffer;
  size_t _size;
};

class RawDataWeak final : public RawData {
 public:
  RawDataWeak(void* buffer, size_t size) : _buffer(buffer), _size(size) {}

  const void* getBuffer() const override { return _buffer; }
  void* getBuffer() override { return _buffer; }
  size_t getSize() const override { return _size; }

 private:
  void* _buffer;
  size_t _size;
};

}  // namespace urx
