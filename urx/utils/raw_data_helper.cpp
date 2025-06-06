#include <complex>
#include <cstdint>
#include <stdexcept>

#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/utils/export.h>
#include <urx/utils/raw_data_helper.h>

namespace urx::utils {

template <template <typename> class Container>
std::shared_ptr<urx::RawData> rawDataFactory(DataType data_type, SamplingType sampling_type,
                                             size_t size) {
  switch (data_type) {
    case DataType::INT16: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<Container<int16_t>>(size);
      }
      return std::make_shared<Container<std::complex<int16_t>>>(size);
      break;
    }
    case DataType::INT32: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<Container<int32_t>>(size);
      }
      return std::make_shared<Container<std::complex<int32_t>>>(size);
      break;
    }
    case DataType::FLOAT: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<Container<float>>(size);
      }
      return std::make_shared<Container<std::complex<float>>>(size);
      break;
    }
    case DataType::DOUBLE: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<Container<double>>(size);
      }
      return std::make_shared<Container<std::complex<double>>>(size);
      break;
    }
    default:
      throw std::runtime_error(__FUNCTION__);
  }
}

template URX_UTILS_EXPORT std::shared_ptr<urx::RawData> rawDataFactory<RawDataVector>(
    DataType data_type, SamplingType sampling_type, size_t size);
template URX_UTILS_EXPORT std::shared_ptr<urx::RawData> rawDataFactory<RawDataNoInit>(
    DataType data_type, SamplingType sampling_type, size_t size);
template URX_UTILS_EXPORT std::shared_ptr<urx::RawData> rawDataFactory<RawDataStream>(
    DataType data_type, SamplingType sampling_type, size_t size);

template <template <typename> class Container>
std::shared_ptr<urx::RawData> rawDataFactory(DataType data_type, SamplingType sampling_type,
                                             size_t size, void* buffer) {
  switch (data_type) {
    case DataType::INT16: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<Container<int16_t>>(buffer, size);
      }
      return std::make_shared<Container<std::complex<int16_t>>>(buffer, size);
      break;
    }
    case DataType::INT32: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<Container<int32_t>>(buffer, size);
      }
      return std::make_shared<Container<std::complex<int32_t>>>(buffer, size);
      break;
    }
    case DataType::FLOAT: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<Container<float>>(buffer, size);
      }
      return std::make_shared<Container<std::complex<float>>>(buffer, size);
      break;
    }
    case DataType::DOUBLE: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<Container<double>>(buffer, size);
      }
      return std::make_shared<Container<std::complex<double>>>(buffer, size);
      break;
    }
    default:
      throw std::runtime_error(__FUNCTION__);
  }
}

template URX_UTILS_EXPORT std::shared_ptr<urx::RawData> rawDataFactory<RawDataWeak>(
    DataType data_type, SamplingType sampling_type, size_t size, void* buffer);

}  // namespace urx::utils
