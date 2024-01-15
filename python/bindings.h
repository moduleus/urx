#pragma once

#include <pybind11/pybind11.h>

#include <urx/vector.h>
#include <urx/group.h>
#include <urx/element_geometry.h>
#include <urx/impulse_response.h>
#include <urx/element.h>
#include <urx/excitation.h>
#include <urx/event.h>
#include <urx/probe.h>
#include <urx/wave.h>
#include <urx/group_data.h>

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

namespace urx::python::detail {
void registerVector(pybind11::module_& m);
}
