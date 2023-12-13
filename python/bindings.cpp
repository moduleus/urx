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
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/urx.h>
#include <urx/version.h>
#include <urx_utils/group_helper.h>

namespace py = pybind11;

using VecFloat32 = std::vector<float>;
using VecFloat64 = std::vector<double>;

using VecVector3D = std::vector<urx::Vector3D<double>>;
using VecGroupPtr = std::vector<std::shared_ptr<urx::Group>>;

// PYBIND11_MAKE_OPAQUE(VecFloat32);
// PYBIND11_MAKE_OPAQUE(VecFloat64);

PYBIND11_MAKE_OPAQUE(VecGroupPtr);

// PYBIND11_MAKE_OPAQUE(urx::Vector3D<double>);
PYBIND11_MAKE_OPAQUE(VecVector3D);
// PYBIND11_MAKE_OPAQUE(urx::DoubleNan);

struct A {
  bool operator==(const A &other) const = default;
  int x;
  double y;
};

struct B {
  bool operator==(const B &other) const = default;
  double z;
  A a;
};

struct C {
  bool operator==(const C &other) const = default;
  std::vector<B> b;
};

// PYBIND11_MAKE_OPAQUE(A);
// PYBIND11_MAKE_OPAQUE(B);
PYBIND11_MAKE_OPAQUE(std::vector<B>);
// PYBIND11_MAKE_OPAQUE(C);

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

class El {
 public:
  El() = delete;
  explicit El(int v) : a(v) {}

  int a;
};

std::ostream &operator<<(std::ostream &s, El const &v) {
  s << "El{" << v.a << '}';
  return s;
}

// NOLINTBEGIN(misc-redundant-expression)
PYBIND11_MODULE(bindings, m) {
  m.doc() = "Variant C++ binding POC";

  // py::bind_vector<VecFloat32>(m, "VecFloat32", py::buffer_protocol());
  // py::bind_vector<VecFloat64>(m, "VecFloat64", py::buffer_protocol());

  // test_vector_int
  py::bind_vector<std::vector<unsigned int>>(m, "VectorInt", py::buffer_protocol());

  // test_vector_custom
  py::class_<El>(m, "El").def(py::init<int>());
  py::bind_vector<std::vector<El>>(m, "VectorEl");
  py::bind_vector<std::vector<std::vector<El>>>(m, "VectorVectorEl");

  py::bind_vector<VecGroupPtr>(m, "VecGroupPtr");
  // py::bind_vector<VecVector3D>(m, "VecVector3D");

  py::bind_vector<std::vector<B>>(m, "VecB");
  // PYBIND11_NUMPY_DTYPE(A, x, y);
  // PYBIND11_NUMPY_DTYPE(B, z, a);
  // PYBIND11_NUMPY_DTYPE(C, b);

  py::class_<VecVector3D>(m, "VecVector3D")
      .def(py::init<>())
      .def("clear", &VecVector3D::clear)
      .def("pop_back", &VecVector3D::pop_back)
      .def("__len__", [](const VecVector3D &v) { return v.size(); })
      .def(
          "__iter__", [](VecVector3D &v) { return py::make_iterator(v.begin(), v.end()); },
          py::keep_alive<0, 1>()); /* Keep vector alive while iterator is used */

  py::class_<A>(m, "A")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("x", &A::x)
      .def_readwrite("y", &A::y);
  py::class_<B>(m, "B")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("z", &B::z)
      .def_readwrite("a", &B::a);
  py::class_<C>(m, "C")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("b", &C::b);
  // .def_property(
  //     "b", [](C &self) { return &self.b; },
  //     // [](C &self, const py::list &b) {
  //     //   B *b_ptr = b.cast<B *>;
  //     //   self.b = std::vector<B>(b_ptr, b_ptr + 2);
  //     // }),
  //     [](C &self, const py::array_t<B> &b) {
  //       self.b = std::vector<B>(b.data(), b.data() + b.size());
  //     });

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
      .def(py::init())
      .def_readwrite("value", &urx::DoubleNan::value)
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
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
      .def(py::init(
          [](double sampling_frequency, double time_offset, const std::string &units, py::list l) {
            std::vector<double> vec = l.cast<std::vector<double>>();
            return urx::ImpulseResponse(urx::DoubleNan(sampling_frequency),
                                        urx::DoubleNan(time_offset), units, vec);
          }))
      .def(py::init())
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
      .def(py::init(
          [](const py::list &vec) { return urx::ElementGeometry(vec.cast<VecVector3D>()); }))
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
      .def(py::init([](const urx::Vector3D<double> &r, const urx::Vector3D<double> &t) {
        return urx::Transform(r, t);
      }))
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("rotation", &urx::Transform::rotation)
      .def_readwrite("translation", &urx::Transform::translation);

  // Element
  py::class_<urx::Element, std::shared_ptr<urx::Element>>(m, "Element")
      .def(py::init(
          [](const urx::Transform &t, const std::weak_ptr<urx::ElementGeometry> &eg,
             const std::weak_ptr<urx::ImpulseResponse> &ir) { return urx::Element(t, eg, ir); }))
      .def(py::init())
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
      .def_readwrite("impulse_response", &urx::Element::impulse_response);

  // Group
  py::class_<urx::Group, std::shared_ptr<urx::Group>>(m, "Group")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("data_type", &urx::Group::data_type)
      .def_readwrite("sampling_type", &urx::Group::sampling_type)
      .def_readwrite("description", &urx::Group::description);

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
