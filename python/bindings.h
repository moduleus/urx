#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

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
#include <urx/wave.h>

using VecFloat64 = std::vector<double>;
using VecVecFloat64 = std::vector<std::vector<double>>;
using VecUInt32 = std::vector<uint32_t>;
using VecVecUInt32 = std::vector<std::vector<uint32_t>>;

using VecVector3D = std::vector<urx::Vector3D<double>>;
using VecGroupPtr = std::vector<std::shared_ptr<urx::Group>>;
using VecElementGeometryPtr = std::vector<std::shared_ptr<urx::ElementGeometry>>;
using VecImpulseResponsePtr = std::vector<std::shared_ptr<urx::ImpulseResponse>>;
using VecElement = std::vector<urx::Element>;
using VecExcitationPtr = std::vector<std::shared_ptr<urx::Excitation>>;
using VecEvent = std::vector<urx::Event>;
using VecProbePtr = std::vector<std::shared_ptr<urx::Probe>>;
using VecWavePtr = std::vector<std::shared_ptr<urx::Wave>>;
using VecGroupData = std::vector<urx::GroupData>;

PYBIND11_MAKE_OPAQUE(VecFloat64);
PYBIND11_MAKE_OPAQUE(VecVecFloat64);
PYBIND11_MAKE_OPAQUE(VecUInt32);
PYBIND11_MAKE_OPAQUE(VecVecUInt32);

PYBIND11_MAKE_OPAQUE(VecGroupPtr);
PYBIND11_MAKE_OPAQUE(VecElementGeometryPtr);
PYBIND11_MAKE_OPAQUE(VecImpulseResponsePtr);
PYBIND11_MAKE_OPAQUE(VecElement);
PYBIND11_MAKE_OPAQUE(VecExcitationPtr);
PYBIND11_MAKE_OPAQUE(VecEvent);
PYBIND11_MAKE_OPAQUE(VecVector3D);
PYBIND11_MAKE_OPAQUE(VecProbePtr);
PYBIND11_MAKE_OPAQUE(VecWavePtr);
PYBIND11_MAKE_OPAQUE(VecGroupData);

namespace urx::python::detail {
void registerVector(pybind11::module_& m);
}
