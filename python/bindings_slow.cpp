#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <pybind11/attr.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl_bind.h>

#include "bindings.h"

#include <urx/detail/compare.h>

namespace py = pybind11;

namespace urx::python::detail {
// void registerVector(pybind11::module_& m) {
//   py::bindVector<VecFloat64>(m, "VecFloat64", py::buffer_protocol());
//   py::implicitly_convertible<py::list, VecFloat64>();
//   py::bindVector<VecVecFloat64>(m, "VecVecFloat64");
//   py::implicitly_convertible<py::list, VecVecFloat64>();
//   py::bindVector<VecUInt32>(m, "VecUInt32", py::buffer_protocol());
//   py::implicitly_convertible<py::list, VecUInt32>();
//   py::bindVector<VecVecUInt32>(m, "VecVecUInt32");
//   py::implicitly_convertible<py::list, VecVecUInt32>();

//   py::bindVector<VecGroupPtr>(m, "VecGroupPtr");
//   py::implicitly_convertible<py::list, VecGroupPtr>();
//   py::bindVector<VecVector3D>(m, "VecVector3D");
//   py::implicitly_convertible<py::list, VecVector3D>();

//   py::bindVector<VecElementGeometryPtr>(m, "VecElementGeometryPtr");
//   py::implicitly_convertible<py::list, VecElementGeometryPtr>();
//   py::bindVector<VecImpulseResponsePtr>(m, "VecImpulseResponsePtr");
//   py::implicitly_convertible<py::list, VecImpulseResponsePtr>();
//   py::bindVector<VecElement>(m, "VecElement");
//   py::implicitly_convertible<py::list, VecElement>();
//   py::bindVector<VecExcitationPtr>(m, "VecExcitationPtr");
//   py::implicitly_convertible<py::list, VecExcitationPtr>();
//   py::bindVector<VecEvent>(m, "VecEvent");
//   py::implicitly_convertible<py::list, VecEvent>();
//   py::bindVector<VecProbePtr>(m, "VecProbePtr");
//   py::implicitly_convertible<py::list, VecProbePtr>();
//   py::bindVector<VecGroupData>(m, "VecGroupData");
//   py::implicitly_convertible<py::list, VecGroupData>();
// }
template <typename CppClass, const char* python_name>
void bindVector(pybind11::module_& m) {
  py::bind_vector<CppClass>(m, python_name, py::buffer_protocol());
  py::implicitly_convertible<py::list, CppClass>();
}

template <typename CppClass, const char* python_name>
void bindVectorNoBufferProtocol(pybind11::module_& m) {
  py::bind_vector<CppClass>(m, python_name);
  py::implicitly_convertible<py::list, CppClass>();
}

namespace {
constexpr const char VEC_UINT8[] = "VecUInt8";
constexpr const char VEC_UINT16[] = "VecUInt16";
constexpr const char VEC_UINT32[] = "VecUInt32";
constexpr const char VEC_VEC_UINT32[] = "VecVecUInt32";
constexpr const char VEC_UINT64[] = "VecUInt64";
constexpr const char VEC_INT8[] = "VecInt8";
constexpr const char VEC_INT16[] = "VecInt16";
constexpr const char VEC_INT32[] = "VecInt32";
constexpr const char VEC_INT64[] = "VecInt64";
constexpr const char VEC_FLOAT32[] = "VecFloat32";
constexpr const char VEC_FLOAT64[] = "VecFloat64";
constexpr const char VEC_VEC_FLOAT64[] = "VecVecFloat64";
constexpr const char VEC_STRING[] = "VecString";
constexpr const char VEC_GROUP_PTR[] = "VecGroupPtr";
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
void bindVector<VecUInt16>(pybind11::module_& m) {
  bindVector<VecUInt16, VEC_UINT16>(m);
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
void bindVector<VecInt8>(pybind11::module_& m) {
  bindVector<VecInt8, VEC_INT8>(m);
}
template <>
void bindVector<VecInt16>(pybind11::module_& m) {
  bindVector<VecInt16, VEC_INT16>(m);
}
template <>
void bindVector<VecInt32>(pybind11::module_& m) {
  bindVector<VecInt32, VEC_INT32>(m);
}
template <>
void bindVector<VecInt64>(pybind11::module_& m) {
  bindVector<VecInt64, VEC_INT64>(m);
}
template <>
void bindVector<VecFloat32>(pybind11::module_& m) {
  bindVector<VecFloat32, VEC_FLOAT32>(m);
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
void bindVector<VecGroupPtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecGroupPtr, VEC_GROUP_PTR>(m);
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

}  // namespace urx::python::detail
