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
constexpr const char VEC_FLOAT32[] = "VecFloat32";
constexpr const char VEC_VEC_FLOAT32[] = "VecVecFloat32";
constexpr const char VEC_FLOAT64[] = "VecFloat64";
constexpr const char VEC_VEC_FLOAT64[] = "VecVecFloat64";
}  // namespace

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

}  // namespace urx::python::detail
