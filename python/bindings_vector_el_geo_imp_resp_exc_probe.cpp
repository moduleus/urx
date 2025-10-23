#include <algorithm>
#include <complex>
#include <cstdint>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <pybind11/pybind11.h>

#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/python/bindings.h>
#include <urx/python/export.h>
#include <urx/utils/group_helper.h>

namespace urx::python::detail {

namespace {
constexpr const char VEC_ELEMENT_GEOMETRY_PTR[] = "VecElementGeometryPtr";
constexpr const char VEC_IMPULSE_RESPONSE_PTR[] = "VecImpulseResponsePtr";
constexpr const char VEC_EXCITATION_PTR[] = "VecExcitationPtr";
constexpr const char VEC_PROBE_PTR[] = "VecProbePtr";
}  // namespace

template <>
URX_PYTHON_EXPORT void bindVector<VecElementGeometryPtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecElementGeometryPtr, VEC_ELEMENT_GEOMETRY_PTR>(m);
}
template <>
URX_PYTHON_EXPORT void bindVector<VecImpulseResponsePtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecImpulseResponsePtr, VEC_IMPULSE_RESPONSE_PTR>(m);
}
template <>
URX_PYTHON_EXPORT void bindVector<VecExcitationPtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecExcitationPtr, VEC_EXCITATION_PTR>(m);
}
template <>
URX_PYTHON_EXPORT void bindVector<VecProbePtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecProbePtr, VEC_PROBE_PTR>(m);
}

}  // namespace urx::python::detail
