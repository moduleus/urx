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
constexpr const char VEC_EVENT[] = "VecEvent";
constexpr const char VEC_GROUP_DATA[] = "VecGroupData";
constexpr const char VEC_GROUP_PTR[] = "VecGroupPtr";
constexpr const char VEC_ELEMENT[] = "VecElement";
}  // namespace

template <>
void bindVector<VecEvent>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecEvent, VEC_EVENT>(m);
}
template <>
void bindVector<VecGroupData>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecGroupData, VEC_GROUP_DATA>(m);
}
template <>
void bindVector<VecGroupPtr>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecGroupPtr, VEC_GROUP_PTR>(m);
}
template <>
void bindVector<VecElement>(pybind11::module_& m) {
  bindVectorNoBufferProtocol<VecElement, VEC_ELEMENT>(m);
}

}  // namespace urx::python::detail
