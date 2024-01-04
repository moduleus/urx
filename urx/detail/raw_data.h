#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <urx/group.h>

namespace urx {

template <typename T>
struct is_complex_t : public std::false_type {};

template <typename T>
struct is_complex_t<std::complex<T>> : public std::true_type {};

template <typename T>
struct is_complex_t<std::vector<std::complex<T>>> : public std::true_type {};

template <typename T>
constexpr bool is_complex() {
  return is_complex_t<T>::value;
}

template <typename T>
constexpr bool is_complex(const T&) {
  return is_complex<T>();
}

class RawData {
 public:
  virtual const void* getBuffer() const = 0;
  virtual void* getBuffer() = 0;
  virtual size_t getSize() const = 0;
  virtual constexpr Group::SamplingType getSamplingType() const = 0;
  virtual Group::DataType getDataType() const = 0;

  bool operator==(const RawData& other) const {
    static std::unordered_map<Group::DataType, size_t> group_dt_to_sizeof{
        {Group::DataType::INT16, sizeof(int16_t)},
        {Group::DataType::INT32, sizeof(int32_t)},
        {Group::DataType::FLOAT, sizeof(float)},
        {Group::DataType::DOUBLE, sizeof(double)}};
    return std::memcmp(getBuffer(), other.getBuffer(),
                       getSize() * group_dt_to_sizeof.at(getDataType()) *
                           (getSamplingType() == Group::SamplingType::RF ? 1 : 2)) == 0;
  }

 protected:
  virtual ~RawData() = default;
};

template <typename T>
class IRawData : public RawData {
 public:
  using value_type = T;

  Group::SamplingType getSamplingType() const override {
    return is_complex_t<value_type>::value ? Group::SamplingType::IQ : Group::SamplingType::RF;
  };

  Group::DataType getDataType() const override {
    std::type_index type = typeid(value_type);
    if constexpr (is_complex_t<value_type>::value) {
      type = typeid(typename value_type::value_type);
    }
    static std::unordered_map<std::type_index, Group::DataType> typeid_to_dt{
        {std::type_index(typeid(int16_t)), Group::DataType::INT16},
        {std::type_index(typeid(int32_t)), Group::DataType::INT32},
        {std::type_index(typeid(float)), Group::DataType::FLOAT},
        {std::type_index(typeid(double)), Group::DataType::DOUBLE}};

    return typeid_to_dt.at(type);
  };

 protected:
  virtual ~IRawData() = default;
};

template <typename DataType>
class RawDataVector final : public IRawData<DataType> {
 public:
  explicit RawDataVector(std::vector<DataType>&& vector) : _vector(std::move(vector)) {}

  const void* getBuffer() const override { return _vector.data(); }
  void* getBuffer() override { return _vector.data(); }
  size_t getSize() const override { return _vector.size(); }

 private:
  std::vector<DataType> _vector;
};

template <typename DataType>
class RawDataNoInit final : public IRawData<DataType> {
 public:
  explicit RawDataNoInit(size_t size)
      : _buffer(std::make_unique_for_overwrite<DataType[]>(size)), _size(size) {}

  const void* getBuffer() const override { return _buffer.get(); }
  void* getBuffer() override { return _buffer.get(); }
  size_t getSize() const override { return _size; }

 private:
  std::unique_ptr<DataType[]> _buffer;
  size_t _size;
};

template <typename DataType>
class RawDataWeak final : public IRawData<DataType> {
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
