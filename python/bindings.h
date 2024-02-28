#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <pybind11/cast.h>
#include <pybind11/pybind11.h>

#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/vector.h>

using VecUInt8 = std::vector<uint8_t>;
using VecUInt16 = std::vector<uint16_t>;
using VecUInt32 = std::vector<uint32_t>;
using VecVecUInt32 = std::vector<std::vector<uint32_t>>;
using VecUInt64 = std::vector<uint64_t>;
using VecInt8 = std::vector<int8_t>;
using VecInt16 = std::vector<int16_t>;
using VecInt32 = std::vector<int32_t>;
using VecInt64 = std::vector<int64_t>;
using VecFloat32 = std::vector<float>;
using VecFloat64 = std::vector<double>;
using VecVecFloat64 = std::vector<std::vector<double>>;
using VecString = std::vector<std::string>;

using VecGroupPtr = std::vector<std::shared_ptr<urx::Group>>;
using VecVector3D = std::vector<urx::Vector3D<double>>;

using VecElementGeometryPtr = std::vector<std::shared_ptr<urx::ElementGeometry>>;
using VecImpulseResponsePtr = std::vector<std::shared_ptr<urx::ImpulseResponse>>;
using VecElement = std::vector<urx::Element>;
using VecExcitationPtr = std::vector<std::shared_ptr<urx::Excitation>>;
using VecEvent = std::vector<urx::Event>;
using VecProbePtr = std::vector<std::shared_ptr<urx::Probe>>;
using VecGroupData = std::vector<urx::GroupData>;

PYBIND11_MAKE_OPAQUE(VecUInt8);
PYBIND11_MAKE_OPAQUE(VecUInt16);
PYBIND11_MAKE_OPAQUE(VecUInt32);
PYBIND11_MAKE_OPAQUE(VecVecUInt32);
PYBIND11_MAKE_OPAQUE(VecUInt64);
PYBIND11_MAKE_OPAQUE(VecInt8);
PYBIND11_MAKE_OPAQUE(VecInt16);
PYBIND11_MAKE_OPAQUE(VecInt32);
PYBIND11_MAKE_OPAQUE(VecInt64);
PYBIND11_MAKE_OPAQUE(VecFloat32);
PYBIND11_MAKE_OPAQUE(VecFloat64);
PYBIND11_MAKE_OPAQUE(VecVecFloat64);
PYBIND11_MAKE_OPAQUE(VecString);

PYBIND11_MAKE_OPAQUE(VecGroupPtr);
PYBIND11_MAKE_OPAQUE(VecVector3D);

PYBIND11_MAKE_OPAQUE(VecElementGeometryPtr);
PYBIND11_MAKE_OPAQUE(VecImpulseResponsePtr);
PYBIND11_MAKE_OPAQUE(VecElement);
PYBIND11_MAKE_OPAQUE(VecExcitationPtr);
PYBIND11_MAKE_OPAQUE(VecEvent);
PYBIND11_MAKE_OPAQUE(VecProbePtr);
PYBIND11_MAKE_OPAQUE(VecGroupData);

namespace urx::python::detail {
template <typename CppClass>
void bindVector(pybind11::module_& m);
}  // namespace urx::python::detail
