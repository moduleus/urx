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
constexpr const char VEC_STRING[] = "VecString";
constexpr const char VEC_VEC_STRING[] = "VecVecString";
constexpr const char VEC_VECTOR2D[] = "VecVector2D";
constexpr const char VEC_VECTOR3D[] = "VecVector3D";
}  // namespace

template <>
void bindVector<VecString>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecString, VEC_STRING>(m);
}
template <>
void bindVector<VecVecString>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVecString, VEC_VEC_STRING>(m);
}
template <>
void bindVector<VecVector2D>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVector2D, VEC_VECTOR2D>(m);
}
template <>
void bindVector<VecVector3D>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecVector3D, VEC_VECTOR3D>(m);
}

}  // namespace urx::python::detail
