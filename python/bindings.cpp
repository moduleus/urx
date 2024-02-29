#include "bindings.h"

#include <algorithm>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <pybind11/buffer_info.h>
#include <pybind11/cast.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>       // IWYU pragma: keep
#include <pybind11/stl_bind.h>  // IWYU pragma: keep

#include <urx/acquisition.h>
#include <urx/config.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
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

  // urx::python::detail::registerVector(m);
  urx::python::detail::bindVector<VecUInt8>(m);
  urx::python::detail::bindVector<VecUInt16>(m);
  urx::python::detail::bindVector<VecUInt32>(m);
  urx::python::detail::bindVector<VecVecUInt32>(m);
  urx::python::detail::bindVector<VecUInt64>(m);
  urx::python::detail::bindVector<VecInt8>(m);
  urx::python::detail::bindVector<VecInt16>(m);
  urx::python::detail::bindVector<VecInt32>(m);
  urx::python::detail::bindVector<VecInt64>(m);
  urx::python::detail::bindVector<VecFloat32>(m);
  urx::python::detail::bindVector<VecFloat64>(m);
  urx::python::detail::bindVector<VecVecFloat64>(m);
  urx::python::detail::bindVector<VecString>(m);

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

  // DoubleNan
  py::class_<urx::DoubleNan>(m, "DoubleNan")
      .def(py::init<double>())
      .def(py::init<const urx::DoubleNan &>())
      .def(py::init())
      .def_readwrite("value", &urx::DoubleNan::value)
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def(double() == pybind11::self)
      .def(double() != pybind11::self)

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

  // ImpulseResponse
  py::class_<urx::ImpulseResponse, std::shared_ptr<urx::ImpulseResponse>>(m, "ImpulseResponse")
      .def(py::init())
      .def(py::init<const urx::ImpulseResponse &>())
      .def(py::init([](const urx::DoubleNan &sampling_frequency, const urx::DoubleNan &time_offset,
                       const std::string &units, const VecFloat64 &vec) {
        return urx::ImpulseResponse{sampling_frequency, time_offset, units, vec};
      }))
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("sampling_frequency", &urx::ImpulseResponse::sampling_frequency)
      .def_readwrite("time_offset", &urx::ImpulseResponse::time_offset)
      .def_readwrite("units", &urx::ImpulseResponse::units)
      .def_readwrite("data", &urx::ImpulseResponse::data);
  urx::python::detail::bindVector<VecImpulseResponsePtr>(m);

  // Vector3D
  py::class_<urx::Vector3D<double>>(m, "Vector3D")
      .def(py::init<double, double, double>())
      .def(py::init<const urx::Vector3D<double> &>())
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("x", &urx::Vector3D<double>::x)
      .def_readwrite("y", &urx::Vector3D<double>::y)
      .def_readwrite("z", &urx::Vector3D<double>::z);
  urx::python::detail::bindVector<VecVector3D>(m);

  // Vector2D
  py::class_<urx::Vector2D<double>>(m, "Vector2D")
      .def(py::init<double, double>())
      .def(py::init<const urx::Vector2D<double> &>())
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("x", &urx::Vector2D<double>::x)
      .def_readwrite("y", &urx::Vector2D<double>::y);

  // Version constants
  m.attr("URX_VERSION_MAJOR") = urx::URX_VERSION_MAJOR;
  m.attr("URX_VERSION_MINOR") = urx::URX_VERSION_MINOR;
  m.attr("URX_VERSION_PATCH") = urx::URX_VERSION_PATCH;

  // Version
  py::class_<urx::Version>(m, "Version")
      .def(py::init<uint16_t, uint16_t, uint16_t>())
      .def(py::init<const urx::Version &>())
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("major", &urx::Version::major)
      .def_readwrite("minor", &urx::Version::minor)
      .def_readwrite("patch", &urx::Version::patch);

  // ElementGeometry
  py::class_<urx::ElementGeometry, std::shared_ptr<urx::ElementGeometry>>(m, "ElementGeometry")
      .def(py::init<const VecVector3D &>())
      .def(py::init<const urx::ElementGeometry &>())
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("perimeter", &urx::ElementGeometry::perimeter);
  urx::python::detail::bindVector<VecElementGeometryPtr>(m);

  // Transform
  py::class_<urx::Transform>(m, "Transform")
      .def(py::init())
      .def(py::init<const urx::Vector3D<double> &, const urx::Vector3D<double> &>())
      .def(py::init<const urx::Transform &>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("rotation", &urx::Transform::rotation)
      .def_readwrite("translation", &urx::Transform::translation);

  // Element
  py::class_<urx::Element, std::shared_ptr<urx::Element>>(m, "Element")
      .def(py::init())
      .def(py::init<const urx::Element &>())
      .def(py::init<const urx::Transform &, const std::shared_ptr<urx::ElementGeometry> &,
                    const std::shared_ptr<urx::ImpulseResponse> &>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("transform", &urx::Element::transform)
      .def_property(
          "element_geometry",
          [](urx::Element &self) {
            if (self.element_geometry.expired()) {
              throw std::runtime_error(
                  "Current element_geometry doesn't reference any ElementGeometry.\n");
            }
            return self.element_geometry.lock();
          },
          [](urx::Element &self, const std::shared_ptr<urx::ElementGeometry> &element_geometry) {
            self.element_geometry = element_geometry;
          })
      .def_property(
          "impulse_response",
          [](urx::Element &self) {
            if (self.impulse_response.expired()) {
              throw std::runtime_error(
                  "Current impulse_response doesn't reference any ImpulseResponse.\n");
            }
            return self.impulse_response.lock();
          },
          [](urx::Element &self, const std::shared_ptr<urx::ImpulseResponse> &impulse_response) {
            self.impulse_response = impulse_response;
          });
  urx::python::detail::bindVector<VecElement>(m);

  // Excitation
  py::class_<urx::Excitation, std::shared_ptr<urx::Excitation>>(m, "Excitation")
      .def(py::init())
      .def(py::init<const urx::Excitation &>())
      .def(py::init([](const std::string &pulse_shape, const urx::DoubleNan &transmit_frequency,
                       const urx::DoubleNan &sampling_frequency, const VecFloat64 &vec) {
        return urx::Excitation{pulse_shape, transmit_frequency, sampling_frequency, vec};
      }))
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("pulse_shape", &urx::Excitation::pulse_shape)
      .def_readwrite("transmit_frequency", &urx::Excitation::transmit_frequency)
      .def_readwrite("sampling_frequency", &urx::Excitation::sampling_frequency)
      .def_readwrite("waveform", &urx::Excitation::waveform);
  urx::python::detail::bindVector<VecExcitationPtr>(m);

  // ProbeType
  py::enum_<urx::ProbeType>(m, "ProbeType")
      .value("LINEAR", urx::ProbeType::LINEAR)
      .value("CURVILINEAR", urx::ProbeType::CURVILINEAR)
      .value("RCA", urx::ProbeType::RCA)
      .value("MATRIX", urx::ProbeType::MATRIX)
      .value("SPARSE", urx::ProbeType::SPARSE)
      .value("UNDEFINED", urx::ProbeType::UNDEFINED);

  // Probe
  py::class_<urx::Probe, std::shared_ptr<urx::Probe>>(m, "Probe")
      .def(py::init())
      .def(py::init<const urx::Probe &>())
      .def(py::init<const std::string &, urx::ProbeType, const urx::Transform &,
                    const VecElementGeometryPtr &, const VecImpulseResponsePtr &,
                    const VecElement &>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("description", &urx::Probe::description)
      .def_readwrite("type", &urx::Probe::type)
      .def_readwrite("transform", &urx::Probe::transform)
      .def_readwrite("element_geometries", &urx::Probe::element_geometries)
      .def_readwrite("impulse_responses", &urx::Probe::impulse_responses)
      .def_readwrite("elements", &urx::Probe::elements);
  urx::python::detail::bindVector<VecProbePtr>(m);

  // WaveType
  py::enum_<urx::WaveType>(m, "WaveType")
      .value("CONVERGING_WAVE", urx::WaveType::CONVERGING_WAVE)
      .value("DIVERGING_WAVE", urx::WaveType::DIVERGING_WAVE)
      .value("PLANE_WAVE", urx::WaveType::PLANE_WAVE)
      .value("CYLINDRICAL_WAVE", urx::WaveType::CYLINDRICAL_WAVE)
      .value("UNDEFINED", urx::WaveType::UNDEFINED);

  // Wave
  py::class_<urx::Wave, std::shared_ptr<urx::Wave>>(m, "Wave")
      .def(py::init())
      .def(py::init<const urx::Wave &>())
      .def(py::init([](const urx::WaveType &type, const urx::DoubleNan &time_zero,
                       const urx::Vector3D<double> &time_zero_reference_point,
                       const VecFloat64 &parameters) {
        return urx::Wave{type, time_zero, time_zero_reference_point, parameters};
      }))
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("type", &urx::Wave::type)
      .def_readwrite("time_zero", &urx::Wave::time_zero)
      .def_readwrite("time_zero_reference_point", &urx::Wave::time_zero_reference_point)
      .def_readwrite("parameters", &urx::Wave::parameters);

  // TransmitSetup
  py::class_<urx::TransmitSetup, std::shared_ptr<urx::TransmitSetup>>(m, "TransmitSetup")
      .def(py::init())
      .def(py::init<const urx::TransmitSetup &>())
      .def(py::init([](const std::shared_ptr<urx::Probe> &probe, const urx::Wave &wave,
                       const VecVecUInt32 &active_elements, const VecExcitationPtr &excitations,
                       const VecFloat64 &delays, const urx::Transform &probe_transform,
                       const urx::DoubleNan &time_offset) {
        const std::vector<std::weak_ptr<urx::Excitation>> excitations_weak(excitations.begin(),
                                                                           excitations.end());
        return urx::TransmitSetup{
            probe, wave, active_elements, excitations_weak, delays, probe_transform, time_offset};
      }))
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_property(
          "probe",
          [](urx::TransmitSetup &self) {
            if (self.probe.expired()) {
              throw std::runtime_error("Current probe doesn't reference any Probe.\n");
            }
            return self.probe.lock();
          },
          [](urx::TransmitSetup &self, const std::shared_ptr<urx::Probe> &probe) {
            self.probe = probe;
          })
      .def_readwrite("wave", &urx::TransmitSetup::wave)
      .def_readwrite("active_elements", &urx::TransmitSetup::active_elements)
      .def_property(
          "excitations",
          [](urx::TransmitSetup &self) {
            VecExcitationPtr shared_vector;
            std::transform(self.excitations.begin(), self.excitations.end(),
                           std::back_inserter(shared_vector),
                           [](const std::weak_ptr<urx::Excitation> &ex_weak_ptr) {
                             if (auto ex_shared_ptr = ex_weak_ptr.lock()) {
                               return ex_shared_ptr;
                             }
                             return std::shared_ptr<urx::Excitation>();
                           });

            return shared_vector;
          },
          [](urx::TransmitSetup &self, const VecExcitationPtr &channel_excitations_shared) {
            self.excitations = std::vector<std::weak_ptr<urx::Excitation>>(
                channel_excitations_shared.begin(), channel_excitations_shared.end());
          })
      .def_readwrite("delays", &urx::TransmitSetup::delays)
      .def_readwrite("probe_transform", &urx::TransmitSetup::probe_transform)
      .def_readwrite("time_offset", &urx::TransmitSetup::time_offset);

  // ReceiveSetup
  py::class_<urx::ReceiveSetup, std::shared_ptr<urx::ReceiveSetup>>(m, "ReceiveSetup")
      .def(py::init())
      .def(py::init<const urx::ReceiveSetup &>())
      .def(py::init(
          [](const std::shared_ptr<urx::Probe> &probe, const urx::Transform &probe_transform,
             const urx::DoubleNan &sampling_frequency, uint32_t number_samples,
             const VecVecUInt32 &active_elements, const VecFloat64 &tgc_profile,
             const urx::DoubleNan &tgc_sampling_frequency,
             const urx::DoubleNan &modulation_frequency, const urx::DoubleNan &time_offset) {
            return urx::ReceiveSetup{
                probe,           probe_transform, sampling_frequency,     number_samples,
                active_elements, tgc_profile,     tgc_sampling_frequency, modulation_frequency,
                time_offset};
          }))
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_property(
          "probe",
          [](urx::ReceiveSetup &self) {
            if (self.probe.expired()) {
              throw std::runtime_error("Current probe doesn't reference any Probe.\n");
            }
            return self.probe.lock();
          },
          [](urx::ReceiveSetup &self, const std::shared_ptr<urx::Probe> &probe) {
            self.probe = probe;
          })
      .def_readwrite("probe_transform", &urx::ReceiveSetup::probe_transform)
      .def_readwrite("sampling_frequency", &urx::ReceiveSetup::sampling_frequency)
      .def_readwrite("number_samples", &urx::ReceiveSetup::number_samples)
      .def_readwrite("active_elements", &urx::ReceiveSetup::active_elements)
      .def_readwrite("tgc_profile", &urx::ReceiveSetup::tgc_profile)
      .def_readwrite("tgc_sampling_frequency", &urx::ReceiveSetup::tgc_sampling_frequency)
      .def_readwrite("modulation_frequency", &urx::ReceiveSetup::modulation_frequency)
      .def_readwrite("time_offset", &urx::ReceiveSetup::time_offset);

  // Event
  py::class_<urx::Event, std::shared_ptr<urx::Event>>(m, "Event")
      .def(py::init())
      .def(py::init<const urx::Event &>())
      .def(py::init<const urx::TransmitSetup &, const urx::ReceiveSetup &>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("transmit_setup", &urx::Event::transmit_setup)
      .def_readwrite("receive_setup", &urx::Event::receive_setup);
  urx::python::detail::bindVector<VecEvent>(m);

  // Group
  py::class_<urx::Group, std::shared_ptr<urx::Group>>(m, "Group")
      .def(py::init())
      .def(py::init<const urx::Group &>())
      .def(py::init<urx::SamplingType, urx::DataType, const std::string &, const VecEvent &>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("sampling_type", &urx::Group::sampling_type)
      .def_readwrite("data_type", &urx::Group::data_type)
      .def_readwrite("description", &urx::Group::description)
      .def_readwrite("sequence", &urx::Group::sequence);
  urx::python::detail::bindVector<VecGroupPtr>(m);

  // GroupData
  py::class_<urx::GroupData, std::shared_ptr<urx::GroupData>>(m, "GroupData")
      .def(py::init())
      .def(py::init<const urx::GroupData &>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_property(
          "group",
          [](urx::GroupData &self) {
            if (self.group.expired()) {
              throw std::runtime_error("Current group is not referenced by the acquisition.\n");
            }
            return self.group.lock();
          },
          [](urx::GroupData &self, const std::shared_ptr<urx::Group> &group) {
            self.group = group;
          })
      .def_property(
          "raw_data",
          [](urx::GroupData &self) {
            const bool are_data_complex = self.raw_data->getSamplingType() == urx::SamplingType::IQ;
            const py::ssize_t data_size = self.raw_data->getSize();
            void *data_ptr = self.raw_data->getBuffer();
            const py::ssize_t sizeof_data_type_var =
                urx::utils::group_helper::sizeofDataType(self.raw_data->getDataType());
            const std::string data_format =
                urx::utils::group_helper::pyGetFormat(self.raw_data->getDataType());

            auto buffer = py::buffer_info(
                data_ptr, sizeof_data_type_var, data_format, are_data_complex ? 2 : 1,
                are_data_complex ? std::vector<py::ssize_t>{data_size, 2}
                                 : std::vector<py::ssize_t>{data_size},
                are_data_complex
                    ? std::vector<py::ssize_t>{sizeof_data_type_var * 2, sizeof_data_type_var}
                    : std::vector<py::ssize_t>{sizeof_data_type_var});

            return py::array(buffer, py::cast(self.raw_data->getBuffer()));
          },
          [](urx::GroupData &self, const py::buffer &vec) {
            py::buffer_info info = vec.request();
            if (info.ndim > 2)
              throw std::runtime_error("Dimension error: Too many dimensions in this data array");

            if (info.ndim == 2) {
              if (info.shape[1] > 2)
                throw std::runtime_error(
                    "Dimension error: Too many data in second dimension (2nd dimension must be "
                    "equal to 2)");
              if (info.shape[1] < 2)
                throw std::runtime_error(
                    "Dimension error: Not enough data in second dimension (2nd dimension must be "
                    "equal to 2)");
            }

            if (info.ndim == 1) {
              if (info.item_type_is_equivalent_to<std::complex<double>>()) {
                self.raw_data = std::make_shared<urx::RawDataWeak<std::complex<double>>>(
                    info.ptr, info.shape[0]);
              } else if (info.item_type_is_equivalent_to<std::complex<float>>()) {
                self.raw_data = std::make_shared<urx::RawDataWeak<std::complex<float>>>(
                    info.ptr, info.shape[0]);
              } else if (info.item_type_is_equivalent_to<int16_t>()) {
                self.raw_data =
                    std::make_shared<urx::RawDataWeak<int16_t>>(info.ptr, info.shape[0]);
              } else if (info.item_type_is_equivalent_to<int32_t>()) {
                self.raw_data =
                    std::make_shared<urx::RawDataWeak<int32_t>>(info.ptr, info.shape[0]);
              } else if (info.item_type_is_equivalent_to<float>()) {
                self.raw_data = std::make_shared<urx::RawDataWeak<float>>(info.ptr, info.shape[0]);
              } else if (info.item_type_is_equivalent_to<double>()) {
                self.raw_data = std::make_shared<urx::RawDataWeak<double>>(info.ptr, info.shape[0]);
              } else {
                throw std::runtime_error("No equivalent data type to provided buffer");
              }
            } else {  // info.ndim == 2
              if (info.item_type_is_equivalent_to<int16_t>()) {
                self.raw_data = std::make_shared<urx::RawDataWeak<std::complex<int16_t>>>(
                    info.ptr, info.shape[0]);
              } else if (info.item_type_is_equivalent_to<int32_t>()) {
                self.raw_data = std::make_shared<urx::RawDataWeak<std::complex<int32_t>>>(
                    info.ptr, info.shape[0]);
              } else if (info.item_type_is_equivalent_to<float>()) {
                self.raw_data = std::make_shared<urx::RawDataWeak<std::complex<float>>>(
                    info.ptr, info.shape[0]);
              } else if (info.item_type_is_equivalent_to<double>()) {
                self.raw_data = std::make_shared<urx::RawDataWeak<std::complex<double>>>(
                    info.ptr, info.shape[0]);
              } else {
                throw std::runtime_error("No equivalent data type to provided buffer");
              }
            }
            // if (info.format)
            //   self.raw_data = std::make_shared<urx::RawDataWeak<float>>(info.ptr, info.shape[0]);
          })
      .def_readwrite("group_timestamp", &urx::GroupData::group_timestamp)
      .def_readwrite("sequence_timestamps", &urx::GroupData::sequence_timestamps)
      .def_readwrite("event_timestamps", &urx::GroupData::event_timestamps);
  urx::python::detail::bindVector<VecGroupData>(m);

  py::class_<urx::Acquisition>(m, "Acquisition")
      .def(py::init())
      .def(py::init<const urx::Acquisition &>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("authors", &urx::Acquisition::authors)
      .def_readwrite("description", &urx::Acquisition::description)
      .def_readwrite("local_time", &urx::Acquisition::local_time)
      .def_readwrite("country_code", &urx::Acquisition::country_code)
      .def_readwrite("system", &urx::Acquisition::system)
      .def_readwrite("sound_speed", &urx::Acquisition::sound_speed)
      .def_readwrite("timestamp", &urx::Acquisition::timestamp)
      .def_readwrite("probes", &urx::Acquisition::probes)
      .def_readwrite("excitations", &urx::Acquisition::excitations)
      .def_readwrite("groups", &urx::Acquisition::groups)
      .def_readwrite("groups_data", &urx::Acquisition::groups_data);

  py::class_<urx::Dataset, std::shared_ptr<urx::Dataset>>(m, "Dataset")
      .def(py::init())
      .def(py::init<const urx::Dataset &>())
      .def(py::init<const urx::Acquisition &, const urx::Version &>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("version", &urx::Dataset::version)
      .def_readwrite("acquisition", &urx::Dataset::acquisition);

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
