#include "bindings_module.h"

#include <cstddef>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>       // IWYU pragma: keep
#include <pybind11/stl_bind.h>  // IWYU pragma: keep
#include <pyerrors.h>

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
#include <urx/python/bindings.h>
#include <urx/python/utils/group_helper.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/urx.h>
#include <urx/utils/exception.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/group_helper.h>
#include <urx/utils/time_helper.h>
#include <urx/vector.h>
#include <urx/version.h>

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

  py::enum_<urx::SamplingType>(m, "UrxSamplingType")
      .value("RF", urx::SamplingType::RF)
      .value("IQ", urx::SamplingType::IQ)
      .value("UNDEFINED", urx::SamplingType::UNDEFINED);
  m.attr("SamplingType") = m.attr("UrxSamplingType");

  py::enum_<urx::DataType>(m, "UrxDataType")
      .value("INT16", urx::DataType::INT16)
      .value("INT32", urx::DataType::INT32)
      .value("FLOAT", urx::DataType::FLOAT)
      .value("DOUBLE", urx::DataType::DOUBLE)
      .value("UNDEFINED", urx::DataType::UNDEFINED);
  m.attr("DataType") = m.attr("UrxDataType");

  py::enum_<urx::ProbeType>(m, "UrxProbeType")
      .value("LINEAR", urx::ProbeType::LINEAR)
      .value("CURVILINEAR", urx::ProbeType::CURVILINEAR)
      .value("RCA", urx::ProbeType::RCA)
      .value("MATRIX", urx::ProbeType::MATRIX)
      .value("SPARSE", urx::ProbeType::SPARSE)
      .value("UNDEFINED", urx::ProbeType::UNDEFINED);
  m.attr("ProbeType") = m.attr("UrxProbeType");

  py::enum_<urx::WaveType>(m, "UrxWaveType")
      .value("CONVERGING_WAVE", urx::WaveType::CONVERGING_WAVE)
      .value("DIVERGING_WAVE", urx::WaveType::DIVERGING_WAVE)
      .value("PLANE_WAVE", urx::WaveType::PLANE_WAVE)
      .value("CYLINDRICAL_WAVE", urx::WaveType::CYLINDRICAL_WAVE)
      .value("UNDEFINED", urx::WaveType::UNDEFINED);
  m.attr("WaveType") = m.attr("UrxWaveType");

  py::class_<urx::DoubleNan>(m, "UrxDoubleNan")
      .def(py::init<double>())
      .def(py::init<const urx::DoubleNan &>())
      .def(py::init())
      .def("__float__", [](const urx::DoubleNan &v) { return v.value; })
      .def("__array__",
           [](const urx::DoubleNan &v) {
             std::vector<double> data = {v.value};
             return py::array_t<double>(data.size(), data.data());
           })
      .def_readwrite("value", &urx::DoubleNan::value)
      .def("__repr__",
           [](const urx::DoubleNan &v) {
             std::ostringstream oss;
             oss << std::setprecision(17) << v.value;
             return oss.str();
           })
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
  m.attr("DoubleNan") = m.attr("UrxDoubleNan");

  urx::python::registerImpulseResponse<urx::ImpulseResponse>(m, "Urx").def(
      py::init([](const urx::DoubleNan &sampling_frequency, const urx::DoubleNan &time_offset,
                  const std::string &units, const urx::python::VecFloat64 &vec) {
        return urx::ImpulseResponse{sampling_frequency, time_offset, units, vec};
      }));
  urx::python::detail::bindVector<urx::python::VecImpulseResponsePtr>(m);

  urx::python::registerVector3D<urx::Vector3D<double>>(m, "Urx");
  urx::python::detail::bindVector<urx::python::VecVector3D>(m);

  urx::python::registerVector2D<urx::Vector2D<double>>(m, "Urx");
  urx::python::detail::bindVector<urx::python::VecVector2D>(m);

  // Version constants
  m.attr("URX_VERSION_MAJOR") = urx::URX_VERSION_MAJOR;
  m.attr("URX_VERSION_MINOR") = urx::URX_VERSION_MINOR;
  m.attr("URX_VERSION_PATCH") = urx::URX_VERSION_PATCH;

  urx::python::registerVersion<urx::Version>(m, "Urx");

  urx::python::registerElementGeometry<urx::ElementGeometry>(m, "Urx").def(
      py::init<const urx::python::VecVector3D &>());
  urx::python::detail::bindVector<urx::python::VecElementGeometryPtr>(m);

  urx::python::registerTransform<urx::Transform>(m, "Urx").def(
      py::init<const urx::Vector3D<double> &, const urx::Vector3D<double> &>());

  urx::python::registerElement<urx::Element>(m, "Urx").def(
      py::init<const urx::Transform &, const std::shared_ptr<urx::ElementGeometry> &,
               const std::shared_ptr<urx::ImpulseResponse> &>());
  urx::python::detail::bindVector<urx::python::VecElement>(m);

  urx::python::registerExcitation<urx::Excitation>(m, "Urx").def(
      py::init<const std::string &, const urx::DoubleNan &, const urx::DoubleNan &,
               const urx::python::VecFloat64 &>());
  urx::python::detail::bindVector<urx::python::VecExcitationPtr>(m);

  urx::python::registerProbe(m);
  urx::python::detail::bindVector<urx::python::VecProbePtr>(m);

  urx::python::registerWave(m);

  urx::python::registerTransmitSetup(m);

  urx::python::registerReceiveSetup(m);

  urx::python::registerEvent<urx::Event>(m, "Urx").def(
      py::init<const urx::TransmitSetup &, const urx::ReceiveSetup &>());
  urx::python::detail::bindVector<urx::python::VecEvent>(m);

  urx::python::registerGroup<urx::Group>(m, "Urx").def(
      py::init<urx::SamplingType, urx::DataType, const std::string &, const urx::DoubleNan &,
               const urx::python::VecEvent &>());
  urx::python::detail::bindVector<urx::python::VecGroupPtr>(m);

  urx::python::registerGroupData(m);
  urx::python::detail::bindVector<urx::python::VecGroupData>(m);

  urx::python::registerAcquisition(m);

  urx::python::registerDataset<urx::Dataset>(m, "Urx").def(
      py::init<const urx::Acquisition &, const urx::Version &>());

  py::register_exception<urx::utils::ReadFileException>(m, "ReadFileException", PyExc_RuntimeError);
  py::register_exception<urx::utils::WriteFileException>(m, "WriteFileException",
                                                         PyExc_RuntimeError);

#ifdef URX_WITH_HDF5
  m.def("loadFromFile", &urx::utils::io::reader::loadFromFile);
  m.def("saveToFile", static_cast<void (*)(const std::string &, const urx::Dataset &)>(
                          &urx::utils::io::writer::saveToFile));
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
  m.def("pyGetFormat", &urx::python::utils::pyGetFormat);
  m.def("sizeofSample", &urx::utils::group_helper::sizeofSample);

  // time_helper.h
  m.def("isIso8601", &urx::utils::time_helper::isIso8601);
  m.def("isIso3166", &urx::utils::time_helper::isIso3166);
}
// NOLINTEND(misc-redundant-expression)
