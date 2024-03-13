#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>       // IWYU pragma: keep
#include <pybind11/stl_bind.h>  // IWYU pragma: keep

#include <urx/acquisition.h>
#include <urx/config.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/python/bindings.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/urx.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/group_helper.h>
#include <urx/utils/time_helper.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

#ifdef URX_WITH_HDF5
#include <urx/utils/io/reader.h>
#include <urx/utils/io/writer.h>
#endif

namespace py = pybind11;

// NOLINTBEGIN(misc-redundant-expression)
PYBIND11_MODULE(bindings, m) {
  m.doc() = "Python binding of urx library";

  urx::python::detail::bindVector<urx::python::VecUInt8>(m);
  urx::python::detail::bindVector<urx::python::VecVecUInt8>(m);
  urx::python::detail::bindVector<urx::python::VecUInt16>(m);
  urx::python::detail::bindVector<urx::python::VecVecUInt16>(m);
  urx::python::detail::bindVector<urx::python::VecUInt32>(m);
  urx::python::detail::bindVector<urx::python::VecVecUInt32>(m);
  urx::python::detail::bindVector<urx::python::VecUInt64>(m);
  urx::python::detail::bindVector<urx::python::VecVecUInt64>(m);
  urx::python::detail::bindVector<urx::python::VecInt8>(m);
  urx::python::detail::bindVector<urx::python::VecVecInt8>(m);
  urx::python::detail::bindVector<urx::python::VecInt16>(m);
  urx::python::detail::bindVector<urx::python::VecVecInt16>(m);
  urx::python::detail::bindVector<urx::python::VecInt32>(m);
  urx::python::detail::bindVector<urx::python::VecVecInt32>(m);
  urx::python::detail::bindVector<urx::python::VecInt64>(m);
  urx::python::detail::bindVector<urx::python::VecVecInt64>(m);
  urx::python::detail::bindVector<urx::python::VecFloat32>(m);
  urx::python::detail::bindVector<urx::python::VecVecFloat32>(m);
  urx::python::detail::bindVector<urx::python::VecFloat64>(m);
  urx::python::detail::bindVector<urx::python::VecVecFloat64>(m);
  urx::python::detail::bindVector<urx::python::VecString>(m);
  urx::python::detail::bindVector<urx::python::VecVecString>(m);

  py::enum_<urx::SamplingType>(m, "SamplingType")
      .value("RF", urx::SamplingType::RF)
      .value("IQ", urx::SamplingType::IQ)
      .value("UNDEFINED", urx::SamplingType::UNDEFINED);

  py::enum_<urx::DataType>(m, "DataType")
      .value("INT16", urx::DataType::INT16)
      .value("INT32", urx::DataType::INT32)
      .value("FLOAT", urx::DataType::FLOAT)
      .value("DOUBLE", urx::DataType::DOUBLE)
      .value("UNDEFINED", urx::DataType::UNDEFINED);

  py::enum_<urx::ProbeType>(m, "ProbeType")
      .value("LINEAR", urx::ProbeType::LINEAR)
      .value("CURVILINEAR", urx::ProbeType::CURVILINEAR)
      .value("RCA", urx::ProbeType::RCA)
      .value("MATRIX", urx::ProbeType::MATRIX)
      .value("SPARSE", urx::ProbeType::SPARSE)
      .value("UNDEFINED", urx::ProbeType::UNDEFINED);

  py::enum_<urx::WaveType>(m, "WaveType")
      .value("CONVERGING_WAVE", urx::WaveType::CONVERGING_WAVE)
      .value("DIVERGING_WAVE", urx::WaveType::DIVERGING_WAVE)
      .value("PLANE_WAVE", urx::WaveType::PLANE_WAVE)
      .value("CYLINDRICAL_WAVE", urx::WaveType::CYLINDRICAL_WAVE)
      .value("UNDEFINED", urx::WaveType::UNDEFINED);

  py::class_<urx::DoubleNan>(m, "DoubleNan")
      .def(py::init<double>())
      .def(py::init<const urx::DoubleNan &>())
      .def(py::init())
      .def_readwrite("value", &urx::DoubleNan::value)
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def(double() == py::self)
      .def(double() != py::self)

      .def(py::self + py::self)
      .def(double() + py::self)
      .def(py::self + double())

      .def(py::self - py::self)
      .def(double() - py::self)
      .def(py::self - double())

      .def(py::self * py::self)
      .def(double() * py::self)
      .def(py::self * double())

      .def(py::self / py::self)
      .def(double() / py::self)
      .def(py::self / double())

      .def(py::self += py::self)
      .def(py::self += double())

      .def(py::self -= py::self)
      .def(py::self -= double())

      .def(py::self *= py::self)
      .def(py::self *= double())

      .def(py::self /= py::self)
      .def(py::self /= double())

      .def(+py::self)
      .def(-py::self);
  py::implicitly_convertible<double, urx::DoubleNan>();
  py::implicitly_convertible<int, urx::DoubleNan>();

  urx::python::registerImpulseResponse<urx::ImpulseResponse>(m).def(
      py::init([](const urx::DoubleNan &sampling_frequency, const urx::DoubleNan &time_offset,
                  const std::string &units, const urx::python::VecFloat64 &vec) {
        return urx::ImpulseResponse{sampling_frequency, time_offset, units, vec};
      }));
  urx::python::detail::bindVector<urx::python::VecImpulseResponsePtr>(m);

  urx::python::registerVector3D<urx::Vector3D<double>>(m);
  urx::python::detail::bindVector<urx::python::VecVector3D>(m);

  urx::python::registerVector2D<urx::Vector2D<double>>(m);
  urx::python::detail::bindVector<urx::python::VecVector2D>(m);

  // Version constants
  m.attr("URX_VERSION_MAJOR") = urx::URX_VERSION_MAJOR;
  m.attr("URX_VERSION_MINOR") = urx::URX_VERSION_MINOR;
  m.attr("URX_VERSION_PATCH") = urx::URX_VERSION_PATCH;

  urx::python::registerVersion<urx::Version>(m);

  urx::python::registerElementGeometry<urx::ElementGeometry>(m).def(
      py::init<const urx::python::VecVector3D &>());
  urx::python::detail::bindVector<urx::python::VecElementGeometryPtr>(m);

  urx::python::registerTransform<urx::Transform>(m).def(
      py::init<const urx::Vector3D<double> &, const urx::Vector3D<double> &>());

  urx::python::registerElement<urx::Element>(m).def(
      py::init<const urx::Transform &, const std::shared_ptr<urx::ElementGeometry> &,
               const std::shared_ptr<urx::ImpulseResponse> &>());
  urx::python::detail::bindVector<urx::python::VecElement>(m);

  urx::python::registerExcitation<urx::Excitation>(m).def(
      py::init<const std::string &, const urx::DoubleNan &, const urx::DoubleNan &,
               const urx::python::VecFloat64 &>());
  urx::python::detail::bindVector<urx::python::VecExcitationPtr>(m);

  urx::python::registerProbe<urx::Probe>(m).def(
      py::init<const std::string &, urx::ProbeType, const urx::Transform &,
               const urx::python::VecElementGeometryPtr &,
               const urx::python::VecImpulseResponsePtr &, const urx::python::VecElement &>());
  urx::python::detail::bindVector<urx::python::VecProbePtr>(m);

  urx::python::registerWave<urx::Wave>(m).def(
      py::init<const urx::WaveType &, const urx::DoubleNan &, const urx::Vector3D<double> &,
               const urx::python::VecFloat64 &>());

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

  urx::python::registerReceiveSetup<urx::ReceiveSetup>(m).def(
      py::init<const std::shared_ptr<urx::Probe> &, const urx::Transform &, const urx::DoubleNan &,
               uint32_t, const urx::python::VecVecUInt32 &, const urx::python::VecFloat64 &,
               const urx::DoubleNan &, const urx::DoubleNan &, const urx::DoubleNan &>());

  urx::python::registerEvent<urx::Event>(m).def(
      py::init<const urx::TransmitSetup &, const urx::ReceiveSetup &>());
  urx::python::detail::bindVector<urx::python::VecEvent>(m);

  urx::python::registerGroup<urx::Group>(m).def(
      py::init<urx::SamplingType, urx::DataType, const std::string &,
               const urx::python::VecEvent &>());
  urx::python::detail::bindVector<urx::python::VecGroupPtr>(m);

  urx::python::registerGroupData<urx::GroupData>(m).def(py::init(
      [](const std::shared_ptr<urx::Group> &group, const py::array &raw_data,
         const urx::DoubleNan &group_timestamp, const std::vector<double> &sequence_timestamps,
         const std::vector<std::vector<double>> &event_timestamps) {
        return urx::GroupData{group, urx::python::detail::pyArrayToRawData(raw_data),
                              group_timestamp, sequence_timestamps, event_timestamps};
      }));
  urx::python::detail::bindVector<urx::python::VecGroupData>(m);

  urx::python::registerAcquisition<urx::Acquisition>(m).def(
      py::init<const std::string &, const std::string &, const std::string &, const std::string &,
               const std::string &, const urx::DoubleNan &, const urx::DoubleNan &,
               const std::vector<std::shared_ptr<urx::Probe>> &,
               const std::vector<std::shared_ptr<urx::Excitation>> &,
               const std::vector<std::shared_ptr<urx::Group>> &,
               const std::vector<urx::GroupData> &>());

  urx::python::registerDataset<urx::Dataset>(m).def(
      py::init<const urx::Acquisition &, const urx::Version &>());

#ifdef URX_WITH_HDF5
  m.def("loadFromFile", &urx::utils::io::reader::loadFromFile);
  m.def("saveToFile", &urx::utils::io::writer::saveToFile);
#endif

  // group_data_reader.h
  py::class_<urx::utils::GroupDataReader>(m, "GroupDataReader")
      .def(py::init<urx::GroupData &, size_t>())
      .def("sequencesCount", &urx::utils::GroupDataReader::sequencesCount)
      .def("eventsCount", &urx::utils::GroupDataReader::eventsCount)
      .def("channelsCount", &urx::utils::GroupDataReader::channelsCount)
      .def("samplesCount", &urx::utils::GroupDataReader::samplesCount)
      .def("offset", &urx::utils::GroupDataReader::offset)
      .def("sampleByteSize", &urx::utils::GroupDataReader::sampleByteSize);

  // group_helper
  m.def("sizeofDataType", &urx::utils::group_helper::sizeofDataType);
  m.def("pyGetFormat", &urx::utils::group_helper::pyGetFormat);
  m.def("sizeofSample", &urx::utils::group_helper::sizeofSample);

  // time_helper.h
  m.def("isIso8601", &urx::utils::time_helper::isIso8601);
  m.def("isIso3166", &urx::utils::time_helper::isIso3166);
}
// NOLINTEND(misc-redundant-expression)
