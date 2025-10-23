#include <complex>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/detail/raw_data.h>
#include <urx/enums.h>

namespace urx::test {

TEST_CASE("RawDataVector", "[test_RawData]") {
  const RawDataVector<double> raw_data_vector_double(std::vector<double>{1.2, 3.4, -5.6});
  const RawDataVector<float> raw_data_vector_float(std::vector<float>{1.2f, 3.4f, -5.6f});
  const RawDataVector<int32_t> raw_data_vector_int32(std::vector<int32_t>{1, 2, -3});
  const RawDataVector<int16_t> raw_data_vector_int16(std::vector<int16_t>{1, 2, -3});

  const RawDataVector<std::complex<double>> raw_data_vector_comp_double(
      std::vector<std::complex<double>>{{1.2, 3.4}, {-5.6, -7.8}});
  const RawDataVector<std::complex<float>> raw_data_vector_comp_float(
      std::vector<std::complex<float>>{{1.2f, 3.4f}, {-5.6f, -7.8f}});
  const RawDataVector<std::complex<int32_t>> raw_data_vector_comp_int32(
      std::vector<std::complex<int32_t>>{{1, 2}, {-3, 4}});
  const RawDataVector<std::complex<int16_t>> raw_data_vector_comp_int16(
      std::vector<std::complex<int16_t>>{{1, 2}, {-3, 4}});

  CHECK(raw_data_vector_double.getDataType() == DataType::DOUBLE);
  CHECK(raw_data_vector_float.getDataType() == DataType::FLOAT);
  CHECK(raw_data_vector_int32.getDataType() == DataType::INT32);
  CHECK(raw_data_vector_int16.getDataType() == DataType::INT16);

  CHECK(raw_data_vector_comp_double.getDataType() == DataType::DOUBLE);
  CHECK(raw_data_vector_comp_float.getDataType() == DataType::FLOAT);
  CHECK(raw_data_vector_comp_int32.getDataType() == DataType::INT32);
  CHECK(raw_data_vector_comp_int16.getDataType() == DataType::INT16);

  CHECK(raw_data_vector_double.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_float.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_int32.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_int16.getSamplingType() == SamplingType::RF);

  CHECK(raw_data_vector_comp_double.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_float.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_int32.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_int16.getSamplingType() == SamplingType::IQ);
}

TEST_CASE("RawDataNoInit", "[test_build]") {
  const RawDataNoInit<double> raw_data_vector_double(42);
  const RawDataNoInit<float> raw_data_vector_float(42);
  const RawDataNoInit<int32_t> raw_data_vector_int32(42);
  const RawDataNoInit<int16_t> raw_data_vector_int16(42);

  const RawDataNoInit<std::complex<double>> raw_data_vector_comp_double(42);
  const RawDataNoInit<std::complex<float>> raw_data_vector_comp_float(42);
  const RawDataNoInit<std::complex<int32_t>> raw_data_vector_comp_int32(42);
  const RawDataNoInit<std::complex<int16_t>> raw_data_vector_comp_int16(42);

  CHECK(raw_data_vector_double.getDataType() == DataType::DOUBLE);
  CHECK(raw_data_vector_float.getDataType() == DataType::FLOAT);
  CHECK(raw_data_vector_int32.getDataType() == DataType::INT32);
  CHECK(raw_data_vector_int16.getDataType() == DataType::INT16);

  CHECK(raw_data_vector_comp_double.getDataType() == DataType::DOUBLE);
  CHECK(raw_data_vector_comp_float.getDataType() == DataType::FLOAT);
  CHECK(raw_data_vector_comp_int32.getDataType() == DataType::INT32);
  CHECK(raw_data_vector_comp_int16.getDataType() == DataType::INT16);

  CHECK(raw_data_vector_double.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_float.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_int32.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_int16.getSamplingType() == SamplingType::RF);

  CHECK(raw_data_vector_comp_double.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_float.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_int32.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_int16.getSamplingType() == SamplingType::IQ);
}

TEST_CASE("RawDataWeak", "[test_build]") {
  std::vector<double> tmp_double(42);
  std::vector<float> tmp_float(42);
  std::vector<int32_t> tmp_int32(42);
  std::vector<int16_t> tmp_int16(42);
  std::vector<std::complex<double>> tmp_comp_double(42);
  std::vector<std::complex<float>> tmp_comp_float(42);
  std::vector<std::complex<int32_t>> tmp_comp_int32(42);
  std::vector<std::complex<int16_t>> tmp_comp_int16(42);

  const RawDataWeak<decltype(tmp_double)::value_type> raw_data_vector_double(
      static_cast<void*>(tmp_double.data()), 1u);
  const RawDataWeak<decltype(tmp_float)::value_type> raw_data_vector_float(
      static_cast<void*>(tmp_float.data()), 1u);
  const RawDataWeak<decltype(tmp_int32)::value_type> raw_data_vector_int32(
      static_cast<void*>(tmp_int32.data()), 1u);
  const RawDataWeak<decltype(tmp_int16)::value_type> raw_data_vector_int16(
      static_cast<void*>(tmp_int16.data()), 1u);

  const RawDataWeak<decltype(tmp_comp_double)::value_type> raw_data_vector_comp_double(
      static_cast<void*>(tmp_comp_double.data()), 1u);
  const RawDataWeak<decltype(tmp_comp_float)::value_type> raw_data_vector_comp_float(
      static_cast<void*>(tmp_comp_float.data()), 1u);
  const RawDataWeak<decltype(tmp_comp_int32)::value_type> raw_data_vector_comp_int32(
      static_cast<void*>(tmp_comp_int32.data()), 1u);
  const RawDataWeak<decltype(tmp_comp_int16)::value_type> raw_data_vector_comp_int16(
      static_cast<void*>(tmp_comp_int16.data()), 1u);

  CHECK(raw_data_vector_double.getDataType() == DataType::DOUBLE);
  CHECK(raw_data_vector_float.getDataType() == DataType::FLOAT);
  CHECK(raw_data_vector_int32.getDataType() == DataType::INT32);
  CHECK(raw_data_vector_int16.getDataType() == DataType::INT16);

  CHECK(raw_data_vector_comp_double.getDataType() == DataType::DOUBLE);
  CHECK(raw_data_vector_comp_float.getDataType() == DataType::FLOAT);
  CHECK(raw_data_vector_comp_int32.getDataType() == DataType::INT32);
  CHECK(raw_data_vector_comp_int16.getDataType() == DataType::INT16);

  CHECK(raw_data_vector_double.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_float.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_int32.getSamplingType() == SamplingType::RF);
  CHECK(raw_data_vector_int16.getSamplingType() == SamplingType::RF);

  CHECK(raw_data_vector_comp_double.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_float.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_int32.getSamplingType() == SamplingType::IQ);
  CHECK(raw_data_vector_comp_int16.getSamplingType() == SamplingType::IQ);
}

TEST_CASE("getDataType throws on wrong template", "[test_RawData]") {
  const RawDataVector<uint32_t> raw_data_vector_uint32(std::vector<uint32_t>{1, 2});
  const RawDataVector<uint16_t> raw_data_vector_uint16(std::vector<uint16_t>{1, 2});

  CHECK(raw_data_vector_uint32.getDataType() == DataType::UNDEFINED);
  CHECK(raw_data_vector_uint16.getDataType() == DataType::UNDEFINED);
}

}  // namespace urx::test
