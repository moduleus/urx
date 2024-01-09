#include <complex>
#include <cstdint>
#include <memory>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx::test {

TEST_CASE("Instantiate all classes to check build", "[test_build]") {
  const Acquisition acquisition;
  const Dataset dataset;
  const DoubleNan double_nan;
  const ElementGeometry element_geometry;
  const Element element;
  const Event event;
  const Excitation excitation;
  const GroupData group_data;
  const Group group;
  const ImpulseResponse impulse_response;
  const Probe probe;
  const ReceiveSetup receive_setup;
  const Transform transform;
  const TransmitSetup transmit_setup;
  const Vector2D<double> vector2D;
  const Vector3D<double> vector3D;
  const Version version;
  const Wave wave;
}

TEST_CASE("Instantiate all RawDataVector classes to check build", "[test_build]") {
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
}

TEST_CASE("Instantiate all RawDataNoInit classes to check build", "[test_build]") {
  const RawDataNoInit<double> raw_data_vector_double(42);
  const RawDataNoInit<float> raw_data_vector_float(42);
  const RawDataNoInit<int32_t> raw_data_vector_int32(42);
  const RawDataNoInit<int16_t> raw_data_vector_int16(42);

  const RawDataNoInit<std::complex<double>> raw_data_vector_comp_double(42);
  const RawDataNoInit<std::complex<float>> raw_data_vector_comp_float(42);
  const RawDataNoInit<std::complex<int32_t>> raw_data_vector_comp_int32(42);
  const RawDataNoInit<std::complex<int16_t>> raw_data_vector_comp_int16(42);
}

TEST_CASE("Instantiate all RawDataWeak classes to check build", "[test_build]") {
  std::vector<double> tmp(42);
  const RawDataWeak<decltype(tmp)::value_type> raw_data_vector_double(
      static_cast<void*>(tmp.data()), 42);
}

}  // namespace urx::test
