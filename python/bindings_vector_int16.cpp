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
constexpr const char VEC_UINT16[] = "VecUInt16";
constexpr const char VEC_VEC_UINT16[] = "VecVecUInt16";
constexpr const char VEC_INT16[] = "VecInt16";
constexpr const char VEC_VEC_INT16[] = "VecVecInt16";
}  // namespace

template <>
URX_PYTHON_EXPORT void bindVector<VecUInt16>(pybind11::module_& m) {
  bindVector<VecUInt16, VEC_UINT16>(m);
}
template <>
URX_PYTHON_EXPORT void bindVector<VecVecUInt16>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecUInt16, VEC_VEC_UINT16>(m);
}
template <>
URX_PYTHON_EXPORT void bindVector<VecInt16>(pybind11::module_& m) {
  bindVector<VecInt16, VEC_INT16>(m);
}
template <>
URX_PYTHON_EXPORT void bindVector<VecVecInt16>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecInt16, VEC_VEC_INT16>(m);
}

}  // namespace urx::python::detail
