#include <algorithm>
#include <complex>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <pybind11/attr.h>
#include <pybind11/buffer_info.h>
#include <pybind11/cast.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/urx.h>
#include <urx/version.h>
#include <urx/wave.h>
#include <urx_utils/group_helper.h>

namespace py = pybind11;

using VecFloat32 = std::vector<float>;
using VecFloat64 = std::vector<double>;
using VecUInt32 = std::vector<uint32_t>;
using VecVecUInt32 = std::vector<std::vector<uint32_t>>;

using VecVector3D = std::vector<urx::Vector3D<double>>;
using VecGroupPtr = std::vector<std::shared_ptr<urx::Group>>;
using VecElementGeometryPtr = std::vector<std::shared_ptr<urx::ElementGeometry>>;
using VecImpulseResponsePtr = std::vector<std::shared_ptr<urx::ImpulseResponse>>;
using VecElement = std::vector<urx::Element>;
using VecExcitationPtr = std::vector<std::shared_ptr<urx::Excitation>>;

// PYBIND11_MAKE_OPAQUE(VecFloat32);
PYBIND11_MAKE_OPAQUE(VecFloat64);
PYBIND11_MAKE_OPAQUE(VecUInt32);
PYBIND11_MAKE_OPAQUE(VecVecUInt32);

PYBIND11_MAKE_OPAQUE(VecGroupPtr);
PYBIND11_MAKE_OPAQUE(VecElementGeometryPtr);
PYBIND11_MAKE_OPAQUE(VecImpulseResponsePtr);
PYBIND11_MAKE_OPAQUE(VecElement);
PYBIND11_MAKE_OPAQUE(VecExcitationPtr);

// PYBIND11_MAKE_OPAQUE(urx::Vector3D<double>);
PYBIND11_MAKE_OPAQUE(VecVector3D);
// PYBIND11_MAKE_OPAQUE(urx::DoubleNan);

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  if (!v.empty()) {
    std::ranges::copy(v, std::ostream_iterator<T>(out, "\n"));
  }
  return out;
}

template <typename T>
struct is_complex_t : public std::false_type {};

template <typename T>
struct is_complex_t<std::complex<T>> : public std::true_type {};

template <typename T>
struct is_complex_t<std::vector<std::complex<T>>> : public std::true_type {};

template <typename T>
constexpr bool is_complex() {
  return is_complex_t<T>::value;
}

template <typename T>
constexpr bool is_complex(const T &v) {
  return is_complex<T>();
}

template <typename T>
constexpr py::ssize_t sizeof_data_type(const std::vector<std::complex<T>> &v) {
  return sizeof(T);
}

template <typename T>
constexpr py::ssize_t sizeof_data_type(const std::vector<T> &v) {
  return sizeof(T);
}

template <typename T>
constexpr std::string get_format(const std::vector<std::complex<T>> &v) {
  return py::format_descriptor<T>::format();
}

template <typename T>
constexpr std::string get_format(const std::vector<T> &v) {
  return py::format_descriptor<T>::format();
}

// NOLINTBEGIN(misc-redundant-expression)
PYBIND11_MODULE(bindings, m) {
  m.doc() = "Variant C++ binding POC";

  // py::bind_vector<VecFloat32>(m, "VecFloat32", py::buffer_protocol());
  py::bind_vector<VecFloat64>(m, "VecFloat64", py::buffer_protocol());
  py::implicitly_convertible<py::list, VecFloat64>();
  py::bind_vector<VecUInt32>(m, "VecUInt32", py::buffer_protocol());
  py::implicitly_convertible<py::list, VecUInt32>();
  py::bind_vector<VecVecUInt32>(m, "VecVecUInt32");
  py::implicitly_convertible<py::list, VecVecUInt32>();

  py::bind_vector<VecGroupPtr>(m, "VecGroupPtr");
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

  py::enum_<urx::Group::SamplingType>(m, "SamplingType")
      .value("RF", urx::Group::SamplingType::RF)
      .value("IQ", urx::Group::SamplingType::IQ)
      .value("UNDEFINED", urx::Group::SamplingType::UNDEFINED);

  py::enum_<urx::Group::DataType>(m, "DataType")
      .value("INT16", urx::Group::DataType::INT16)
      .value("INT32", urx::Group::DataType::INT32)
      .value("FLOAT", urx::Group::DataType::FLOAT)
      .value("DOUBLE", urx::Group::DataType::DOUBLE)
      .value("UNDEFINED", urx::Group::DataType::UNDEFINED);

  // DoubleNan
  py::class_<urx::DoubleNan>(m, "DoubleNan")
      .def(py::init<double>())
      .def(py::init<urx::DoubleNan>())
      .def(py::init())
      .def_readwrite("value", &urx::DoubleNan::value)
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def(pybind11::self == double())
      .def(pybind11::self != double())
      .def(double() == pybind11::self)
      .def(double() != pybind11::self)
      .def(py::self + py::self)
      .def(py::self += py::self)
      .def(py::self += double())
      .def(double() + py::self)
      .def(py::self + double())
      .def(py::self -= double())
      .def(double() - py::self)
      .def(py::self - double())
      .def(py::self *= double())
      .def(double() * py::self)
      .def(py::self * double())
      .def(py::self /= double())
      .def(double() / py::self)
      .def(py::self / double())
      .def(+py::self)
      .def(-py::self);

  // ImpulseResponse
  py::class_<urx::ImpulseResponse, std::shared_ptr<urx::ImpulseResponse>>(m, "ImpulseResponse")
      .def(py::init())
      .def(py::init<urx::ImpulseResponse>())
      .def(py::init([](const std::variant<urx::DoubleNan, double> &sampling_frequency,
                       const std::variant<urx::DoubleNan, double> &time_offset,
                       const std::string &units, std::vector<double> vec) {
        return urx::ImpulseResponse(
            std::visit([](auto &&d) { return urx::DoubleNan(d); }, sampling_frequency),
            std::visit([](auto &&d) { return urx::DoubleNan(d); }, time_offset), units, vec);
      }))
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("sampling_frequency", &urx::ImpulseResponse::sampling_frequency)
      .def_readwrite("time_offset", &urx::ImpulseResponse::time_offset)
      .def_readwrite("units", &urx::ImpulseResponse::units)
      .def_readwrite("data", &urx::ImpulseResponse::data);

  // Vector3D
  py::class_<urx::Vector3D<double>>(m, "Vector3D")
      .def(py::init([](double x, double y, double z) { return urx::Vector3D<double>(x, y, z); }))
      .def(
          py::init([](const urx::Vector3D<double> &other) { return urx::Vector3D<double>(other); }))
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("x", &urx::Vector3D<double>::x)
      .def_readwrite("y", &urx::Vector3D<double>::y)
      .def_readwrite("z", &urx::Vector3D<double>::z);

  // Vector2D
  py::class_<urx::Vector2D<double>>(m, "Vector2D")
      .def(py::init([](double x, double y) { return urx::Vector2D<double>(x, y); }))
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
      .def(py::init([](double major, double minor, double patch) {
        return urx::Version(major, minor, patch);
      }))
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("major", &urx::Version::major)
      .def_readwrite("minor", &urx::Version::minor)
      .def_readwrite("patch", &urx::Version::patch);

  // ElementGeometry
  py::class_<urx::ElementGeometry, std::shared_ptr<urx::ElementGeometry>>(m, "ElementGeometry")
      // .def(py::init(
      //     [](const py::list &vec) { return urx::ElementGeometry(vec.cast<VecVector3D>()); }))
      // .def(py::init([](const py::array_t<urx::Vector3D<double>> &vec) {
      //   VecVector3D perimeter = VecVector3D(vec.data(), vec.data() + vec.size());
      //   return urx::ElementGeometry(perimeter);
      // }))
      // .def(py::init<py::list>())
      .def(py::init<VecVector3D>())
      .def(py::init<urx::ElementGeometry>())
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("perimeter", &urx::ElementGeometry::perimeter);
  // .def_property(
  //     "perimeter", [](urx::ElementGeometry &self) { return &self.perimeter; },
  //     [](urx::ElementGeometry &self, const py::list &perimeter) {
  //       self.perimeter = perimeter.cast<VecVector3D>();
  //     });
  // .def_property(
  //     "perimeter", [](urx::ElementGeometry &self) { return &self.perimeter; },
  //     [](urx::ElementGeometry &self, const py::buffer &perimeter) {
  //       py::buffer_info info = perimeter.request();
  //       std::cout << "coucou";
  //       if (info.item_type_is_equivalent_to<urx::Vector3D<double>>()) {
  //         self.perimeter =
  //             VecVector3D(static_cast<urx::Vector3D<double> *>(info.ptr),
  //                         static_cast<urx::Vector3D<double> *>(info.ptr) + info.shape[0]);
  //       } else {
  //         throw std::runtime_error("Incorrect data type. Expected type is double list.");
  //       }
  //     });

  // Transform
  py::class_<urx::Transform>(m, "Transform")
      .def(py::init())
      .def(py::init<urx::Vector3D<double>, urx::Vector3D<double>>())
      .def(py::init<urx::Transform>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("rotation", &urx::Transform::rotation)
      .def_readwrite("translation", &urx::Transform::translation);

  // Element
  py::class_<urx::Element, std::shared_ptr<urx::Element>>(m, "Element")
      .def(py::init())
      .def(py::init<urx::Element>())
      .def(py::init<urx::Transform, std::shared_ptr<urx::ElementGeometry>,
                    std::shared_ptr<urx::ImpulseResponse>>())
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

  // Excitation
  py::class_<urx::Excitation, std::shared_ptr<urx::Excitation>>(m, "Excitation")
      .def(py::init())
      .def(py::init<urx::Excitation>())
      .def(py::init([](const std::string &pulse_shape,
                       const std::variant<urx::DoubleNan, double> &transmit_frequency,
                       const std::variant<urx::DoubleNan, double> &sampling_frequency,
                       std::vector<double> vec) {
        return urx::Excitation(
            pulse_shape, std::visit([](auto &&d) { return urx::DoubleNan(d); }, transmit_frequency),
            std::visit([](auto &&d) { return urx::DoubleNan(d); }, sampling_frequency), vec);
      }))
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("pulse_shape", &urx::Excitation::pulse_shape)
      .def_readwrite("transmit_frequency", &urx::Excitation::transmit_frequency)
      .def_readwrite("sampling_frequency", &urx::Excitation::sampling_frequency)
      .def_readwrite("waveform", &urx::Excitation::waveform);

  // ProbeType
  py::enum_<urx::Probe::ProbeType>(m, "ProbeType")
      .value("LINEAR", urx::Probe::ProbeType::LINEAR)
      .value("CURVILINEAR", urx::Probe::ProbeType::CURVILINEAR)
      .value("RCA", urx::Probe::ProbeType::RCA)
      .value("MATRIX", urx::Probe::ProbeType::MATRIX)
      .value("SPARSE", urx::Probe::ProbeType::SPARSE)
      .value("UNDEFINED", urx::Probe::ProbeType::UNDEFINED);

  // Probe
  py::class_<urx::Probe, std::shared_ptr<urx::Probe>>(m, "Probe")
      .def(py::init())
      .def(py::init<urx::Probe>())
      .def(py::init<std::string, urx::Probe::ProbeType, urx::Transform, VecElementGeometryPtr,
                    VecImpulseResponsePtr, VecElement>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("description", &urx::Probe::description)
      .def_readwrite("type", &urx::Probe::type)
      .def_readwrite("transform", &urx::Probe::transform)
      .def_readwrite("element_geometries", &urx::Probe::element_geometries)
      .def_readwrite("impulse_responses", &urx::Probe::impulse_responses)
      .def_readwrite("elements", &urx::Probe::elements);

  // WaveType
  py::enum_<urx::Wave::WaveType>(m, "WaveType")
      .value("CONVERGING_WAVE", urx::Wave::WaveType::CONVERGING_WAVE)
      .value("DIVERGING_WAVE", urx::Wave::WaveType::DIVERGING_WAVE)
      .value("PLANE_WAVE", urx::Wave::WaveType::PLANE_WAVE)
      .value("CYLINDRICAL_WAVE", urx::Wave::WaveType::CYLINDRICAL_WAVE)
      .value("UNDEFINED", urx::Wave::WaveType::UNDEFINED);

  // Wave
  py::class_<urx::Wave, std::shared_ptr<urx::Wave>>(m, "Wave")
      .def(py::init())
      .def(py::init<urx::Wave>())
      .def(py::init([](urx::Wave::WaveType type, urx::DoubleNan time_zero,
                       urx::Vector3D<double> time_zero_reference_point,
                       VecVecUInt32 channel_mapping, VecExcitationPtr channel_excitations_shared,
                       VecFloat64 channel_delays, VecFloat64 parameters) {
        std::vector<std::weak_ptr<urx::Excitation>> channel_excitations_weak(
            channel_excitations_shared.begin(), channel_excitations_shared.end());
        return urx::Wave(type, time_zero, time_zero_reference_point, channel_mapping,
                         channel_excitations_weak, channel_delays, parameters);
      }))
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("type", &urx::Wave::type)
      .def_readwrite("time_zero", &urx::Wave::time_zero)
      .def_readwrite("time_zero_reference_point", &urx::Wave::time_zero_reference_point)
      .def_readwrite("channel_mapping", &urx::Wave::channel_mapping)
      .def_property(
          "channel_excitations",
          [](urx::Wave &self) {
            std::vector<std::shared_ptr<urx::Excitation>> shared_vector;
            std::transform(self.channel_excitations.begin(), self.channel_excitations.end(),
                           std::back_inserter(shared_vector),
                           [](const std::weak_ptr<urx::Excitation> &ex_weak_ptr) {
                             if (auto ex_shared_ptr = ex_weak_ptr.lock()) {
                               return ex_shared_ptr;
                             } else {
                               return std::shared_ptr<urx::Excitation>();
                             }
                           });

            return shared_vector;
          },
          [](urx::Wave &self,
             const std::vector<std::shared_ptr<urx::Excitation>> &channel_excitations_shared) {
            self.channel_excitations = std::vector<std::weak_ptr<urx::Excitation>>(
                channel_excitations_shared.begin(), channel_excitations_shared.end());
          })
      .def_readwrite("channel_delays", &urx::Wave::channel_delays)
      .def_readwrite("parameters", &urx::Wave::parameters);

  // TransmitSetup
  py::class_<urx::TransmitSetup, std::shared_ptr<urx::TransmitSetup>>(m, "TransmitSetup")
      .def(py::init())
      .def(py::init<urx::TransmitSetup>())
      .def(
          py::init([](const std::shared_ptr<urx::Probe> &probe,
                      const std::shared_ptr<urx::Wave> &wave, const urx::Transform &probe_transform,
                      const std::variant<urx::DoubleNan, double> &time_offset) {
            return urx::TransmitSetup(
                probe, wave, probe_transform,
                std::visit([](auto &&d) { return urx::DoubleNan(d); }, time_offset));
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
      .def_property(
          "wave",
          [](urx::TransmitSetup &self) {
            if (self.wave.expired()) {
              throw std::runtime_error("Current wave doesn't reference any Wave.\n");
            }
            return self.wave.lock();
          },
          [](urx::TransmitSetup &self, const std::shared_ptr<urx::Wave> &wave) {
            self.wave = wave;
          })
      .def_readwrite("probe_transform", &urx::TransmitSetup::probe_transform)
      .def_readwrite("time_offset", &urx::TransmitSetup::time_offset);

  // Group
  py::class_<urx::Group, std::shared_ptr<urx::Group>>(m, "Group")
      .def(py::init())
      .def(py::init<urx::Group>())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("data_type", &urx::Group::data_type)
      .def_readwrite("sampling_type", &urx::Group::sampling_type)
      .def_readwrite("description", &urx::Group::description);

  // GroupData
  py::class_<urx::GroupData, std::shared_ptr<urx::GroupData>>(m, "GroupData")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_property(
          "group",
          [](urx::GroupData &self) {
            if (self.group.expired()) {
              throw std::runtime_error("Current group is not referenced by the acquisition.\n");
              // return std::shared_ptr<urx::Group>(nullptr);
            }
            return self.group.lock();
          },
          [](urx::GroupData &self, const std::shared_ptr<urx::Group> &group) {
            self.group = group;
          })
      .def_readwrite("group_timestamp", &urx::GroupData::group_timestamp)
      // .def_readwrite("sequence_timestamps", &urx::GroupData::sequence_timestamps)
      .def_property(
          "sequence_timestamps",
          [](urx::GroupData &self) {
            return py::array_t<double>(
                py::buffer_info(self.sequence_timestamps.data(), sizeof(double),
                                py::format_descriptor<double>::format(), 1,
                                {self.sequence_timestamps.size()}, {sizeof(double)}),
                py::cast(&self.sequence_timestamps));
          },
          [](urx::GroupData &self, const py::buffer &vec) {
            py::buffer_info info = vec.request();
            if (info.item_type_is_equivalent_to<double>()) {
              // self.sequence_timestamps = VecFloat64(
              //     static_cast<double *>(info.ptr), static_cast<double *>(info.ptr) + info.shape[0]);
            }
          })
      .def_readwrite("event_timestamps", &urx::GroupData::event_timestamps)
      // .def_readwrite("raw_data", &urx::GroupData::raw_data)
      .def_property(
          "raw_data",
          [](urx::GroupData &self) {
            const std::shared_ptr<urx::Group> shared_group = self.group.lock();
            const bool are_data_complex =
                shared_group->sampling_type == urx::Group::SamplingType::IQ;
            const py::ssize_t data_size = self.raw_data->getSize();
            urx::GroupHelper group_helper{*shared_group};
            void *data_ptr = self.raw_data->getBuffer();
            const py::ssize_t sizeof_data_type_var = group_helper.sizeof_data_type();
            const std::string data_format = group_helper.py_get_format();

            auto buffer = py::buffer_info(
                data_ptr, sizeof_data_type_var, data_format, are_data_complex ? 2 : 1,
                are_data_complex ? std::vector<py::ssize_t>{data_size, 2}
                                 : std::vector<py::ssize_t>{data_size},
                are_data_complex
                    ? std::vector<py::ssize_t>{sizeof_data_type_var * 2, sizeof_data_type_var}
                    : std::vector<py::ssize_t>{sizeof_data_type_var});

            return py::array(buffer, py::cast(self.raw_data.get()));
          },
          [](urx::GroupData &self, const py::buffer &vec) {
            py::buffer_info info = vec.request();
            if (info.ndim > 2)
              throw std::runtime_error("Dimension error: Too many dimensions in this data array");

            if (info.ndim == 2 && info.shape[1] != 2)
              throw std::runtime_error("Dimension error: Too many data in second dimension");

            self.raw_data = std::make_shared<urx::RawDataWeak>(info.ptr, info.shape[0]);
          });

  py::class_<urx::Dataset>(m, "Dataset")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("version", &urx::Dataset::version)
      .def_readwrite("acquisition", &urx::Dataset::acquisition);

  py::class_<urx::Acquisition>(m, "Acquisition")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("system", &urx::Acquisition::system)
      .def_readwrite("sound_speed", &urx::Acquisition::sound_speed)
      .def_readwrite("timestamp", &urx::Acquisition::timestamp)
      .def_readwrite("groups", &urx::Acquisition::groups)
      .def_readwrite("group_data", &urx::Acquisition::groups_data);
}
// NOLINTEND(misc-redundant-expression)
