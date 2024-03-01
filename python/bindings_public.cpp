#include <algorithm>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <pybind11/attr.h>
#include <pybind11/cast.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>       // IWYU pragma: keep
#include <pybind11/stl_bind.h>  // IWYU pragma: keep

#include <urx/detail/compare.h>
#include <urx/python/bindings.h>

namespace urx::python::detail {

namespace {
constexpr const char VEC_UINT8[] = "VecUInt8";
constexpr const char VEC_VEC_UINT8[] = "VecVecUInt8";
constexpr const char VEC_UINT16[] = "VecUInt16";
constexpr const char VEC_VEC_UINT16[] = "VecVecUInt16";
constexpr const char VEC_UINT32[] = "VecUInt32";
constexpr const char VEC_VEC_UINT32[] = "VecVecUInt32";
constexpr const char VEC_UINT64[] = "VecUInt64";
constexpr const char VEC_VEC_UINT64[] = "VecVecUInt64";
constexpr const char VEC_INT8[] = "VecInt8";
constexpr const char VEC_VEC_INT8[] = "VecVecInt8";
constexpr const char VEC_INT16[] = "VecInt16";
constexpr const char VEC_VEC_INT16[] = "VecVecInt16";
constexpr const char VEC_INT32[] = "VecInt32";
constexpr const char VEC_VEC_INT32[] = "VecVecInt32";
constexpr const char VEC_INT64[] = "VecInt64";
constexpr const char VEC_VEC_INT64[] = "VecVecInt64";
constexpr const char VEC_FLOAT32[] = "VecFloat32";
constexpr const char VEC_VEC_FLOAT32[] = "VecVecFloat32";
constexpr const char VEC_FLOAT64[] = "VecFloat64";
constexpr const char VEC_VEC_FLOAT64[] = "VecVecFloat64";
constexpr const char VEC_STRING[] = "VecString";
constexpr const char VEC_VEC_STRING[] = "VecVecString";
constexpr const char VEC_GROUP_PTR[] = "VecGroupPtr";
constexpr const char VEC_VECTOR2D[] = "VecVector2D";
constexpr const char VEC_VECTOR3D[] = "VecVector3D";
constexpr const char VEC_ELEMENT_GEOMETRY_PTR[] = "VecElementGeometryPtr";
constexpr const char VEC_IMPULSE_RESPONSE_PTR[] = "VecImpulseResponsePtr";
constexpr const char VEC_ELEMENT[] = "VecElement";
constexpr const char VEC_EXCITATION_PTR[] = "VecExcitationPtr";
constexpr const char VEC_PROBE_PTR[] = "VecProbePtr";
constexpr const char VEC_EVENT[] = "VecEvent";
constexpr const char VEC_GROUP_DATA[] = "VecGroupData";
}  // namespace

template <>
void bindVector<VecUInt8>(pybind11::module_& m) {
  bindVector<VecUInt8, VEC_UINT8>(m);
}
template <>
void bindVector<VecVecUInt8>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecUInt8, VEC_VEC_UINT8>(m);
}
template <>
void bindVector<VecUInt16>(pybind11::module_& m) {
  bindVector<VecUInt16, VEC_UINT16>(m);
}
template <>
void bindVector<VecVecUInt16>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecUInt16, VEC_VEC_UINT16>(m);
}
template <>
void bindVector<VecUInt32>(pybind11::module_& m) {
  bindVector<VecUInt32, VEC_UINT32>(m);
}
template <>
void bindVector<VecVecUInt32>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecUInt32, VEC_VEC_UINT32>(m);
}
template <>
void bindVector<VecUInt64>(pybind11::module_& m) {
  bindVector<VecUInt64, VEC_UINT64>(m);
}
template <>
void bindVector<VecVecUInt64>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecUInt64, VEC_VEC_UINT64>(m);
}
template <>
void bindVector<VecInt8>(pybind11::module_& m) {
  bindVector<VecInt8, VEC_INT8>(m);
}
template <>
void bindVector<VecVecInt8>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecInt8, VEC_VEC_INT8>(m);
}
template <>
void bindVector<VecInt16>(pybind11::module_& m) {
  bindVector<VecInt16, VEC_INT16>(m);
}
template <>
void bindVector<VecVecInt16>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecInt16, VEC_VEC_INT16>(m);
}
template <>
void bindVector<VecInt32>(pybind11::module_& m) {
  bindVector<VecInt32, VEC_INT32>(m);
}
template <>
void bindVector<VecVecInt32>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecInt32, VEC_VEC_INT32>(m);
}
template <>
void bindVector<VecInt64>(pybind11::module_& m) {
  bindVector<VecInt64, VEC_INT64>(m);
}
template <>
void bindVector<VecVecInt64>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecInt64, VEC_VEC_INT64>(m);
}
template <>
void bindVector<VecFloat32>(pybind11::module_& m) {
  bindVector<VecFloat32, VEC_FLOAT32>(m);
}
template <>
void bindVector<VecVecFloat32>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecFloat32, VEC_VEC_FLOAT32>(m);
}
template <>
void bindVector<VecFloat64>(pybind11::module_& m) {
  bindVector<VecFloat64, VEC_FLOAT64>(m);
}
template <>
void bindVector<VecVecFloat64>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecFloat64, VEC_VEC_FLOAT64>(m);
}
template <>
void bindVector<VecString>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecString, VEC_STRING>(m);
}
template <>
void bindVector<VecVecString>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecString, VEC_VEC_STRING>(m);
}
template <>
void bindVector<VecGroupPtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecGroupPtr, VEC_GROUP_PTR>(m);
}
template <>
void bindVector<VecVector2D>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVector2D, VEC_VECTOR2D>(m);
}
template <>
void bindVector<VecVector3D>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVector3D, VEC_VECTOR3D>(m);
}
template <>
void bindVector<VecElementGeometryPtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecElementGeometryPtr, VEC_ELEMENT_GEOMETRY_PTR>(m);
}
template <>
void bindVector<VecImpulseResponsePtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecImpulseResponsePtr, VEC_IMPULSE_RESPONSE_PTR>(m);
}
template <>
void bindVector<VecElement>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecElement, VEC_ELEMENT>(m);
}
template <>
void bindVector<VecExcitationPtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecExcitationPtr, VEC_EXCITATION_PTR>(m);
}
template <>
void bindVector<VecProbePtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecProbePtr, VEC_PROBE_PTR>(m);
}
template <>
void bindVector<VecEvent>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecEvent, VEC_EVENT>(m);
}
template <>
void bindVector<VecGroupData>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecGroupData, VEC_GROUP_DATA>(m);
}

py::array rawDataToPyArray(urx::RawData& raw_data) {
  const bool are_data_complex = raw_data.getSamplingType() == urx::SamplingType::IQ;
  const py::ssize_t data_size = raw_data.getSize();
  void* data_ptr = raw_data.getBuffer();
  const py::ssize_t sizeof_data_type_var =
      urx::utils::group_helper::sizeofDataType(raw_data.getDataType());
  const std::string data_format = urx::utils::group_helper::pyGetFormat(raw_data.getDataType());

  auto buffer = py::buffer_info(
      data_ptr, sizeof_data_type_var, data_format, are_data_complex ? 2 : 1,
      are_data_complex ? std::vector<py::ssize_t>{data_size, 2}
                       : std::vector<py::ssize_t>{data_size},
      are_data_complex ? std::vector<py::ssize_t>{sizeof_data_type_var * 2, sizeof_data_type_var}
                       : std::vector<py::ssize_t>{sizeof_data_type_var});

  return py::array(buffer, py::cast(raw_data.getBuffer()));
}

std::shared_ptr<urx::RawData> pyArrayToRawData(const py::array& array) {
  py::buffer_info info = array.request();
  if (info.ndim > 2)
    throw std::runtime_error("Dimension error: Too many dimensions in this data array");

  if (info.ndim == 2) {
    if (info.shape[1] > 2)
      throw std::runtime_error(
          "Dimension error: Too many data in second dimension (2nd dimension must be "
          "equal to 2)");
    if (info.shape[1] < 2)
      throw std::runtime_error(
          "Dimension error: Not enough data in second dimension (2nd dimension must be "
          "equal to 2)");
  }

  if (info.ndim == 1) {
    if (info.item_type_is_equivalent_to<std::complex<double>>()) {
      return std::make_shared<urx::RawDataWeak<std::complex<double>>>(info.ptr, info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<std::complex<float>>()) {
      return std::make_shared<urx::RawDataWeak<std::complex<float>>>(info.ptr, info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<int16_t>()) {
      return std::make_shared<urx::RawDataWeak<int16_t>>(info.ptr, info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<int32_t>()) {
      return std::make_shared<urx::RawDataWeak<int32_t>>(info.ptr, info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<float>()) {
      return std::make_shared<urx::RawDataWeak<float>>(info.ptr, info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<double>()) {
      return std::make_shared<urx::RawDataWeak<double>>(info.ptr, info.shape[0]);
    }
    throw std::runtime_error("No equivalent data type to provided buffer");
  }
  // info.ndim == 2
  if (info.item_type_is_equivalent_to<int16_t>()) {
    return std::make_shared<urx::RawDataWeak<std::complex<int16_t>>>(info.ptr, info.shape[0]);
  }
  if (info.item_type_is_equivalent_to<int32_t>()) {
    return std::make_shared<urx::RawDataWeak<std::complex<int32_t>>>(info.ptr, info.shape[0]);
  }
  if (info.item_type_is_equivalent_to<float>()) {
    return std::make_shared<urx::RawDataWeak<std::complex<float>>>(info.ptr, info.shape[0]);
  }
  if (info.item_type_is_equivalent_to<double>()) {
    return std::make_shared<urx::RawDataWeak<std::complex<double>>>(info.ptr, info.shape[0]);
  }
  throw std::runtime_error("No equivalent data type to provided buffer");
}

}  // namespace urx::python::detail
