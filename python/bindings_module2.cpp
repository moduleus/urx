#include "bindings_module.h"

#include <urx/acquisition.h>

namespace urx::python {

void registerProbe(py::module_ &m) {
  urx::python::registerProbe<urx::Probe>(m).def(
      py::init<const std::string &, urx::ProbeType, const urx::Transform &,
               const urx::python::VecElementGeometryPtr &,
               const urx::python::VecImpulseResponsePtr &, const urx::python::VecElement &>());
}

void registerWave(py::module_ &m) {
  urx::python::registerWave<urx::Wave>(m).def(
      py::init<const urx::WaveType &, const urx::DoubleNan &, const urx::Vector3D<double> &,
               const urx::python::VecFloat64 &>());
}

void registerTransmitSetup(py::module_ &m) {
  urx::python::registerTransmitSetup<urx::TransmitSetup>(m).def(py::init(
      [](const std::shared_ptr<urx::Probe> &probe, const urx::Wave &wave,
         const urx::python::VecVecUInt32 &active_elements,
         const urx::python::VecExcitationPtr &excitations, const urx::python::VecFloat64 &delays,
         const urx::Transform &probe_transform, const urx::DoubleNan &time_offset) {
        const std::vector<std::weak_ptr<urx::Excitation>> excitations_weak(excitations.begin(),
                                                                           excitations.end());
        return urx::TransmitSetup{
            probe, wave, active_elements, excitations_weak, delays, probe_transform, time_offset};
      }));
}

void registerReceiveSetup(py::module_ &m) {
  urx::python::registerReceiveSetup<urx::ReceiveSetup>(m).def(
      py::init<const std::shared_ptr<urx::Probe> &, const urx::Transform &, const urx::DoubleNan &,
               uint32_t, const urx::python::VecVecUInt32 &, const urx::python::VecFloat64 &,
               const urx::DoubleNan &, const urx::DoubleNan &, const urx::DoubleNan &>());
}

void registerGroupData(py::module_ &m) {
  urx::python::registerGroupData<urx::GroupData>(m).def(py::init(
      [](const std::shared_ptr<urx::Group> &group, const py::array &raw_data,
         const urx::DoubleNan &group_timestamp, const std::vector<double> &sequence_timestamps,
         const std::vector<std::vector<double>> &event_timestamps) {
        return urx::GroupData{group, urx::python::detail::pyArrayToRawData(raw_data),
                              group_timestamp, sequence_timestamps, event_timestamps};
      }));
}

void registerAcquisition(py::module_ &m) {
  urx::python::registerAcquisition<urx::Acquisition>(m).def(
      py::init<const std::string &, const std::string &, const std::string &, const std::string &,
               const std::string &, const urx::DoubleNan &,
               const std::vector<std::shared_ptr<urx::Probe>> &,
               const std::vector<std::shared_ptr<urx::Excitation>> &,
               const std::vector<std::shared_ptr<urx::Group>> &,
               const std::vector<urx::GroupData> &>());
}

}  // namespace urx::python