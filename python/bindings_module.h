#pragma once

#include <pybind11/pybind11.h>

#include <urx/python/bindings.h>

namespace urx::python {

void registerProbe(py::module_ &m);

void registerWave(py::module_ &m);

void registerTransmitSetup(py::module_ &m);

void registerReceiveSetup(py::module_ &m);

void registerGroupData(py::module_ &m);

void registerAcquisition(py::module_ &m);

}  // namespace urx::python