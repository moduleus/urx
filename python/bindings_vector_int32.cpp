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
#include <urx/utils/group_helper.h>

namespace urx::python::detail {

namespace {
constexpr const char VEC_UINT32[] = "VecUInt32";
constexpr const char VEC_VEC_UINT32[] = "VecVecUInt32";
constexpr const char VEC_INT32[] = "VecInt32";
constexpr const char VEC_VEC_INT32[] = "VecVecInt32";
}  // namespace

template <>
void bindVector<VecUInt32>(pybind11::module_& m) {
  bindVector<VecUInt32, VEC_UINT32>(m);
}
template <>
void bindVector<VecVecUInt32>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecUInt32, VEC_VEC_UINT32>(m);
}
template <>
void bindVector<VecInt32>(pybind11::module_& m) {
  bindVector<VecInt32, VEC_INT32>(m);
}
template <>
void bindVector<VecVecInt32>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecInt32, VEC_VEC_INT32>(m);
}

}  // namespace urx::python::detail
