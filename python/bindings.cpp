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
#include <pybind11/stl_bind.h>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/version.h>
#include <urx_utils/group_helper.h>

namespace py = pybind11;

using VecFloat32 = std::vector<float>;
using VecFloat64 = std::vector<double>;

using VecGroup = std::vector<std::shared_ptr<urx::Group>>;

PYBIND11_MAKE_OPAQUE(VecFloat32);
PYBIND11_MAKE_OPAQUE(VecFloat64);

PYBIND11_MAKE_OPAQUE(VecGroup);

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

  py::bind_vector<VecFloat32>(m, "VecFloat32", py::buffer_protocol());
  py::bind_vector<VecFloat64>(m, "VecFloat64", py::buffer_protocol());

  py::bind_vector<VecGroup>(m, "VecGroup");

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
              self.sequence_timestamps = VecFloat64(
                  static_cast<double *>(info.ptr), static_cast<double *>(info.ptr) + info.shape[0]);
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

  py::class_<urx::Version>(m, "Version")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("major", &urx::Version::major)
      .def_readwrite("minor", &urx::Version::minor)
      .def_readwrite("patch", &urx::Version::patch);

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
