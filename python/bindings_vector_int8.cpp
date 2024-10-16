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

#include <urx/detail/compare.h>
#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/python/bindings.h>
#include <urx/python/export.h>
#include <urx/utils/group_helper.h>

namespace urx::python::detail {

namespace {
constexpr const char VEC_UINT8[] = "VecUInt8";
constexpr const char VEC_VEC_UINT8[] = "VecVecUInt8";
constexpr const char VEC_INT8[] = "VecInt8";
constexpr const char VEC_VEC_INT8[] = "VecVecInt8";
}  // namespace

template <>
URX_PYTHON_EXPORT void bindVector<VecUInt8>(pybind11::module_& m) {
  bindVector<VecUInt8, VEC_UINT8>(m);
}
template <>
URX_PYTHON_EXPORT void bindVector<VecVecUInt8>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecUInt8, VEC_VEC_UINT8>(m);
}

template <>
URX_PYTHON_EXPORT void bindVector<VecInt8>(pybind11::module_& m) {
  bindVector<VecInt8, VEC_INT8>(m);
}
template <>
URX_PYTHON_EXPORT void bindVector<VecVecInt8>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecInt8, VEC_VEC_INT8>(m);
}

}  // namespace urx::python::detail
