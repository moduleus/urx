#include "bindings.h"

namespace py = pybind11;

namespace urx::python::detail {
void registerVector(pybind11::module_& m) {
  py::bind_vector<VecFloat64>(m, "VecFloat64", py::buffer_protocol());
  py::implicitly_convertible<py::list, VecFloat64>();
  py::bind_vector<VecVecFloat64>(m, "VecVecFloat64");
  py::implicitly_convertible<py::list, VecVecFloat64>();
  py::bind_vector<VecUInt32>(m, "VecUInt32", py::buffer_protocol());
  py::implicitly_convertible<py::list, VecUInt32>();
  py::bind_vector<VecVecUInt32>(m, "VecVecUInt32");
  py::implicitly_convertible<py::list, VecVecUInt32>();

  py::bind_vector<VecGroupPtr>(m, "VecGroupPtr");
  py::implicitly_convertible<py::list, VecGroupPtr>();
  py::bind_vector<VecVector3D>(m, "VecVector3D");
  py::implicitly_convertible<py::list, VecVector3D>();

  py::bind_vector<VecElementGeometryPtr>(m, "VecElementGeometryPtr");
  py::implicitly_convertible<py::list, VecElementGeometryPtr>();
  py::bind_vector<VecImpulseResponsePtr>(m, "VecImpulseResponsePtr");
  py::implicitly_convertible<py::list, VecImpulseResponsePtr>();
  py::bind_vector<VecElement>(m, "VecElement");
  py::implicitly_convertible<py::list, VecElement>();
  py::bind_vector<VecExcitationPtr>(m, "VecExcitationPtr");
  py::implicitly_convertible<py::list, VecExcitationPtr>();
  py::bind_vector<VecEvent>(m, "VecEvent");
  py::implicitly_convertible<py::list, VecEvent>();
  py::bind_vector<VecProbePtr>(m, "VecProbePtr");
  py::implicitly_convertible<py::list, VecProbePtr>();
  py::bind_vector<VecWavePtr>(m, "VecWavePtr");
  py::implicitly_convertible<py::list, VecWavePtr>();
  py::bind_vector<VecGroupData>(m, "VecGroupData");
  py::implicitly_convertible<py::list, VecGroupData>();
}
}  // namespace urx::python::detail
