#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <pybind11/attr.h>
#include <pybind11/cast.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl_bind.h>

#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/python/export.h>
#include <urx/vector.h>

namespace urx::python {

namespace py = pybind11;

namespace detail {

template <typename CppClass, const char *python_name>
void bindVector(pybind11::module_ &m) {
  py::bind_vector<CppClass>(m, python_name, py::buffer_protocol());
  py::implicitly_convertible<py::list, CppClass>();
}

template <typename CppClass, const char *python_name>
void bindVectorNoBufferProtocol(pybind11::module_ &m) {
  py::bind_vector<CppClass>(m, python_name);
  py::implicitly_convertible<py::list, CppClass>();
}

}  // namespace detail

using VecUInt8 = std::vector<uint8_t>;
using VecVecUInt8 = std::vector<std::vector<uint8_t>>;
using VecUInt16 = std::vector<uint16_t>;
using VecVecUInt16 = std::vector<std::vector<uint16_t>>;
using VecUInt32 = std::vector<uint32_t>;
using VecVecUInt32 = std::vector<std::vector<uint32_t>>;
using VecUInt64 = std::vector<uint64_t>;
using VecVecUInt64 = std::vector<std::vector<uint64_t>>;
using VecInt8 = std::vector<int8_t>;
using VecVecInt8 = std::vector<std::vector<int8_t>>;
using VecInt16 = std::vector<int16_t>;
using VecVecInt16 = std::vector<std::vector<int16_t>>;
using VecInt32 = std::vector<int32_t>;
using VecVecInt32 = std::vector<std::vector<int32_t>>;
using VecInt64 = std::vector<int64_t>;
using VecVecInt64 = std::vector<std::vector<int64_t>>;
using VecFloat32 = std::vector<float>;
using VecVecFloat32 = std::vector<std::vector<float>>;
using VecFloat64 = std::vector<double>;
using VecVecFloat64 = std::vector<std::vector<double>>;
using VecString = std::vector<std::string>;
using VecVecString = std::vector<std::vector<std::string>>;

using VecGroupPtr = std::vector<std::shared_ptr<urx::Group>>;
using VecVector2D = std::vector<urx::Vector2D<double>>;
using VecVector3D = std::vector<urx::Vector3D<double>>;

using VecElementGeometryPtr = std::vector<std::shared_ptr<urx::ElementGeometry>>;
using VecImpulseResponsePtr = std::vector<std::shared_ptr<urx::ImpulseResponse>>;
using VecElement = std::vector<urx::Element>;
using VecExcitationPtr = std::vector<std::shared_ptr<urx::Excitation>>;
using VecEvent = std::vector<urx::Event>;
using VecProbePtr = std::vector<std::shared_ptr<urx::Probe>>;
using VecGroupData = std::vector<urx::GroupData>;

}  // namespace urx::python

PYBIND11_MAKE_OPAQUE(urx::python::VecUInt8);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecUInt8);
PYBIND11_MAKE_OPAQUE(urx::python::VecUInt16);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecUInt16);
PYBIND11_MAKE_OPAQUE(urx::python::VecUInt32);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecUInt32);
PYBIND11_MAKE_OPAQUE(urx::python::VecUInt64);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecUInt64);
PYBIND11_MAKE_OPAQUE(urx::python::VecInt8);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecInt8);
PYBIND11_MAKE_OPAQUE(urx::python::VecInt16);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecInt16);
PYBIND11_MAKE_OPAQUE(urx::python::VecInt32);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecInt32);
PYBIND11_MAKE_OPAQUE(urx::python::VecInt64);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecInt64);
PYBIND11_MAKE_OPAQUE(urx::python::VecFloat32);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecFloat32);
PYBIND11_MAKE_OPAQUE(urx::python::VecFloat64);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecFloat64);
PYBIND11_MAKE_OPAQUE(urx::python::VecString);
PYBIND11_MAKE_OPAQUE(urx::python::VecVecString);

PYBIND11_MAKE_OPAQUE(urx::python::VecGroupPtr);
PYBIND11_MAKE_OPAQUE(urx::python::VecVector2D);
PYBIND11_MAKE_OPAQUE(urx::python::VecVector3D);

PYBIND11_MAKE_OPAQUE(urx::python::VecElementGeometryPtr);
PYBIND11_MAKE_OPAQUE(urx::python::VecImpulseResponsePtr);
PYBIND11_MAKE_OPAQUE(urx::python::VecElement);
PYBIND11_MAKE_OPAQUE(urx::python::VecExcitationPtr);
PYBIND11_MAKE_OPAQUE(urx::python::VecEvent);
PYBIND11_MAKE_OPAQUE(urx::python::VecProbePtr);
PYBIND11_MAKE_OPAQUE(urx::python::VecGroupData);

namespace urx::python {

namespace detail {
template <typename CppClass>
void bindVector(py::module_ &m);

URX_PYTHON_EXPORT py::array rawDataToPyArray(urx::RawData &raw_data);

URX_PYTHON_EXPORT std::shared_ptr<urx::RawData> pyArrayToRawData(const py::array &array);

}  // namespace detail

// NOLINTBEGIN(misc-redundant-expression)
template <typename ImpulseResponse>
py::class_<ImpulseResponse, std::shared_ptr<ImpulseResponse>> registerImpulseResponse(
    py::module_ &m) {
  return py::class_<ImpulseResponse, std::shared_ptr<ImpulseResponse>>(m, "ImpulseResponse",
                                                                       py::module_local())
      .def(py::init())
      .def(py::init<const ImpulseResponse &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("sampling_frequency", &ImpulseResponse::sampling_frequency)
      .def_readwrite("time_offset", &ImpulseResponse::time_offset)
      .def_readwrite("units", &ImpulseResponse::units)
      .def_readwrite("data", &ImpulseResponse::data);
}

template <typename Container>
py::class_<Container> registerVector3D(py::module_ &m) {
  return py::class_<Container>(m, "Vector3D", py::module_local())
      .def(py::init<typename Container::Type, typename Container::Type, typename Container::Type>())
      .def(py::init<const Container &>())
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("x", &Container::x)
      .def_readwrite("y", &Container::y)
      .def_readwrite("z", &Container::z);
}

template <typename Container>
py::class_<Container> registerVector2D(py::module_ &m) {
  return py::class_<Container>(m, "Vector2D", py::module_local())
      .def(py::init<typename Container::Type, typename Container::Type>())
      .def(py::init<const Container &>())
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("x", &Container::x)
      .def_readwrite("y", &Container::y);
}

template <typename Version>
py::class_<Version> registerVersion(py::module_ &m) {
  return py::class_<Version>(m, "Version", py::module_local())
      .def(py::init<uint16_t, uint16_t, uint16_t>())
      .def(py::init<const Version &>())
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("major", &Version::major)
      .def_readwrite("minor", &Version::minor)
      .def_readwrite("patch", &Version::patch);
}

template <typename ElementGeometry>
py::class_<ElementGeometry, std::shared_ptr<ElementGeometry>> registerElementGeometry(
    py::module_ &m) {
  return py::class_<ElementGeometry, std::shared_ptr<ElementGeometry>>(m, "ElementGeometry",
                                                                       py::module_local())
      .def(py::init())
      .def(py::init<const ElementGeometry &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("perimeter", &ElementGeometry::perimeter);
}

template <typename Transform>
py::class_<Transform> registerTransform(py::module_ &m) {
  return py::class_<Transform>(m, "Transform", py::module_local())
      .def(py::init())
      .def(py::init<const Transform &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("rotation", &Transform::rotation)
      .def_readwrite("translation", &Transform::translation);
}

template <typename Element>
py::class_<Element, std::shared_ptr<Element>> registerElement(py::module_ &m) {
  return py::class_<Element, std::shared_ptr<Element>>(m, "Element", py::module_local())
      .def(py::init())
      .def(py::init<const Element &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("transform", &Element::transform)
      .def_property(
          "element_geometry",
          [](Element &self) -> decltype(self.element_geometry.lock()) {
            if (self.element_geometry.expired()) {
              return {};
            }
            return self.element_geometry.lock();
          },
          [](Element &self, const std::shared_ptr<urx::ElementGeometry> &element_geometry) {
            self.element_geometry = element_geometry;
          })
      .def_property(
          "impulse_response",
          [](Element &self) -> decltype(self.impulse_response.lock()) {
            if (self.impulse_response.expired()) {
              return {};
            }
            return self.impulse_response.lock();
          },
          [](Element &self, const std::shared_ptr<urx::ImpulseResponse> &impulse_response) {
            self.impulse_response = impulse_response;
          });
}

template <typename Excitation>
py::class_<Excitation, std::shared_ptr<Excitation>> registerExcitation(py::module_ &m) {
  return py::class_<Excitation, std::shared_ptr<Excitation>>(m, "Excitation", py::module_local())
      .def(py::init())
      .def(py::init<const Excitation &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("pulse_shape", &Excitation::pulse_shape)
      .def_readwrite("transmit_frequency", &Excitation::transmit_frequency)
      .def_readwrite("sampling_frequency", &Excitation::sampling_frequency)
      .def_readwrite("waveform", &Excitation::waveform);
}

template <typename Probe>
py::class_<Probe, std::shared_ptr<Probe>> registerProbe(py::module_ &m) {
  return py::class_<Probe, std::shared_ptr<Probe>>(m, "Probe", py::module_local())
      .def(py::init())
      .def(py::init<const Probe &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("description", &Probe::description)
      .def_readwrite("type", &Probe::type)
      .def_readwrite("transform", &Probe::transform)
      .def_readwrite("element_geometries", &Probe::element_geometries)
      .def_readwrite("impulse_responses", &Probe::impulse_responses)
      .def_readwrite("elements", &Probe::elements);
}

template <typename Wave>
py::class_<Wave, std::shared_ptr<Wave>> registerWave(py::module_ &m) {
  return py::class_<Wave, std::shared_ptr<Wave>>(m, "Wave", py::module_local())
      .def(py::init())
      .def(py::init<const Wave &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("type", &Wave::type)
      .def_readwrite("time_zero", &Wave::time_zero)
      .def_readwrite("time_zero_reference_point", &Wave::time_zero_reference_point)
      .def_readwrite("parameters", &Wave::parameters);
}

template <typename TransmitSetup>
py::class_<TransmitSetup, std::shared_ptr<TransmitSetup>> registerTransmitSetup(py::module_ &m) {
  using Excitation = typename decltype(TransmitSetup::excitations)::value_type::element_type;

  return py::class_<TransmitSetup, std::shared_ptr<TransmitSetup>>(m, "TransmitSetup",
                                                                   py::module_local())
      .def(py::init())
      .def(py::init<const TransmitSetup &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_property(
          "probe",
          [](TransmitSetup &self) -> decltype(self.probe.lock()) {
            if (self.probe.expired()) {
              return {};
            }
            return self.probe.lock();
          },
          [](TransmitSetup &self, const std::shared_ptr<urx::Probe> &probe) { self.probe = probe; })
      .def_readwrite("wave", &TransmitSetup::wave)
      .def_readwrite("active_elements", &TransmitSetup::active_elements)
      .def_property(
          "excitations",
          [](TransmitSetup &self) {
            std::vector<std::shared_ptr<Excitation>> shared_vector;
            std::transform(self.excitations.begin(), self.excitations.end(),
                           std::back_inserter(shared_vector),
                           [](const std::weak_ptr<Excitation> &ex_weak_ptr) {
                             if (auto ex_shared_ptr = ex_weak_ptr.lock()) {
                               return ex_shared_ptr;
                             }
                             return std::shared_ptr<Excitation>();
                           });

            return shared_vector;
          },
          [](TransmitSetup &self,
             const std::vector<std::shared_ptr<Excitation>> &channel_excitations_shared) {
            self.excitations = std::vector<std::weak_ptr<Excitation>>(
                channel_excitations_shared.begin(), channel_excitations_shared.end());
          })
      .def_readwrite("delays", &TransmitSetup::delays)
      .def_readwrite("probe_transform", &TransmitSetup::probe_transform)
      .def_readwrite("time_offset", &TransmitSetup::time_offset);
}

template <typename ReceiveSetup>
py::class_<ReceiveSetup, std::shared_ptr<ReceiveSetup>> registerReceiveSetup(py::module_ &m) {
  return py::class_<ReceiveSetup, std::shared_ptr<ReceiveSetup>>(m, "ReceiveSetup",
                                                                 py::module_local())
      .def(py::init())
      .def(py::init<const ReceiveSetup &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_property(
          "probe",
          [](ReceiveSetup &self) -> decltype(self.probe.lock()) {
            if (self.probe.expired()) {
              return {};
            }
            return self.probe.lock();
          },
          [](ReceiveSetup &self, const std::shared_ptr<urx::Probe> &probe) { self.probe = probe; })
      .def_readwrite("probe_transform", &ReceiveSetup::probe_transform)
      .def_readwrite("sampling_frequency", &ReceiveSetup::sampling_frequency)
      .def_readwrite("number_samples", &ReceiveSetup::number_samples)
      .def_readwrite("active_elements", &ReceiveSetup::active_elements)
      .def_readwrite("tgc_profile", &ReceiveSetup::tgc_profile)
      .def_readwrite("tgc_sampling_frequency", &ReceiveSetup::tgc_sampling_frequency)
      .def_readwrite("modulation_frequency", &ReceiveSetup::modulation_frequency)
      .def_readwrite("time_offset", &ReceiveSetup::time_offset);
}

template <typename Event>
py::class_<Event, std::shared_ptr<Event>> registerEvent(py::module_ &m) {
  return py::class_<Event, std::shared_ptr<Event>>(m, "Event", py::module_local())
      .def(py::init())
      .def(py::init<const Event &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("transmit_setup", &Event::transmit_setup)
      .def_readwrite("receive_setup", &Event::receive_setup);
}

template <typename Group, typename... Options>
py::class_<Group, std::shared_ptr<Group>, Options...> registerGroup(py::module_ &m) {
  return py::class_<Group, std::shared_ptr<Group>, Options...>(m, "Group", py::module_local())
      .def(py::init())
      .def(py::init<const Group &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("sampling_type", &Group::sampling_type)
      .def_readwrite("data_type", &Group::data_type)
      .def_readwrite("description", &Group::description)
      .def_readwrite("sound_speed", &Group::sound_speed)
      .def_readwrite("sequence", &Group::sequence);
}

template <typename GroupData>
py::class_<GroupData, std::shared_ptr<GroupData>> registerGroupData(py::module_ &m) {
  using Group = typename decltype(GroupData::group)::element_type;
  return py::class_<GroupData, std::shared_ptr<GroupData>>(m, "GroupData", py::module_local())
      .def(py::init())
      .def(py::init<const GroupData &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_property(
          "group",
          [](GroupData &self) -> decltype(self.group.lock()) {
            if (self.group.expired()) {
              return {};
            }
            return self.group.lock();
          },
          [](GroupData &self, const std::shared_ptr<Group> &group) { self.group = group; })
      .def_property(
          "raw_data", [](GroupData &self) { return detail::rawDataToPyArray(*self.raw_data); },
          [](GroupData &self, const py::array &vec) {
            self.raw_data = detail::pyArrayToRawData(vec);
          })
      .def_readwrite("group_timestamp", &GroupData::group_timestamp)
      .def_readwrite("sequence_timestamps", &GroupData::sequence_timestamps)
      .def_readwrite("event_timestamps", &GroupData::event_timestamps);
}

template <typename Acquisition>
py::class_<Acquisition> registerAcquisition(py::module_ &m) {
  return py::class_<Acquisition>(m, "Acquisition", py::module_local())
      .def(py::init())
      .def(py::init<const Acquisition &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("authors", &Acquisition::authors)
      .def_readwrite("description", &Acquisition::description)
      .def_readwrite("local_time", &Acquisition::local_time)
      .def_readwrite("country_code", &Acquisition::country_code)
      .def_readwrite("system", &Acquisition::system)
      .def_readwrite("timestamp", &Acquisition::timestamp)
      .def_readwrite("probes", &Acquisition::probes)
      .def_readwrite("excitations", &Acquisition::excitations)
      .def_readwrite("groups", &Acquisition::groups)
      .def_readwrite("groups_data", &Acquisition::groups_data);
}

template <typename Dataset>
py::class_<Dataset, std::shared_ptr<Dataset>> registerDataset(py::module_ &m) {
  return py::class_<Dataset, std::shared_ptr<Dataset>>(m, "Dataset")
      .def(py::init())
      .def(py::init<const Dataset &>())
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def_readwrite("version", &Dataset::version)
      .def_readwrite("acquisition", &Dataset::acquisition);
}
// NOLINTEND(misc-redundant-expression)

}  // namespace urx::python
