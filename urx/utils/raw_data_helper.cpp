#include <complex>
#include <cstdint>
#include <stdexcept>

#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/utils/raw_data_helper.h>

namespace urx::utils {

std::shared_ptr<urx::RawData> rawDataFactory(DataType data_type, SamplingType sampling_type,
                                             size_t size) {
  switch (data_type) {
    case DataType::INT16: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<RawDataNoInit<int16_t>>(size);
      }
      return std::make_shared<RawDataNoInit<std::complex<int16_t>>>(size);
      break;
    }
    case DataType::INT32: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<RawDataNoInit<int32_t>>(size);
      }
      return std::make_shared<RawDataNoInit<std::complex<int32_t>>>(size);
      break;
    }
    case DataType::FLOAT: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<RawDataNoInit<float>>(size);
      }
      return std::make_shared<RawDataNoInit<std::complex<float>>>(size);
      break;
    }
    case DataType::DOUBLE: {
      if (sampling_type == SamplingType::RF) {
        return std::make_shared<RawDataNoInit<double>>(size);
      }
      return std::make_shared<RawDataNoInit<std::complex<double>>>(size);
      break;
    }
    default:
      throw std::runtime_error(__FUNCTION__);
  }
}

}  // namespace urx::utils
