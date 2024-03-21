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
constexpr const char VEC_UINT64[] = "VecUInt64";
constexpr const char VEC_VEC_UINT64[] = "VecVecUInt64";
constexpr const char VEC_INT64[] = "VecInt64";
constexpr const char VEC_VEC_INT64[] = "VecVecInt64";
}  // namespace

template <>
void bindVector<VecInt64>(pybind11::module_& m) {
  bindVector<VecInt64, VEC_INT64>(m);
}
template <>
void bindVector<VecVecInt64>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecInt64, VEC_VEC_INT64>(m);
}
template <>
void bindVector<VecUInt64>(pybind11::module_& m) {
  bindVector<VecUInt64, VEC_UINT64>(m);
}
template <>
void bindVector<VecVecUInt64>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecUInt64, VEC_VEC_UINT64>(m);
}

}  // namespace urx::python::detail
