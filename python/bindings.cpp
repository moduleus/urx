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

#include <uff/acquisition.h>
#include <uff/dataset.h>
#include <uff/detail/compare.h>
#include <uff/detail/double_nan.h>
#include <uff/detail/raw_data.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/version.h>
#include <uff_utils/group_helper.h>

namespace py = pybind11;

using VecFloat32 = std::vector<float>;
using VecFloat64 = std::vector<double>;

using VecGroup = std::vector<std::shared_ptr<uff::Group>>;

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

  py::enum_<uff::Group::SamplingType>(m, "SamplingType")
      .value("RF", uff::Group::SamplingType::RF)
      .value("IQ", uff::Group::SamplingType::IQ)
      .value("UNDEFINED", uff::Group::SamplingType::UNDEFINED);

  py::enum_<uff::Group::DataType>(m, "DataType")
      .value("INT16", uff::Group::DataType::INT16)
      .value("INT32", uff::Group::DataType::INT32)
      .value("FLOAT", uff::Group::DataType::FLOAT)
      .value("DOUBLE", uff::Group::DataType::DOUBLE)
      .value("UNDEFINED", uff::Group::DataType::UNDEFINED);

  py::class_<uff::Group, std::shared_ptr<uff::Group>>(m, "Group")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("data_type", &uff::Group::data_type)
      .def_readwrite("sampling_type", &uff::Group::sampling_type)
      .def_readwrite("description", &uff::Group::description);

  py::class_<uff::GroupData, std::shared_ptr<uff::GroupData>>(m, "GroupData")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_property(
          "group",
          [](uff::GroupData &self) {
            if (self.group.expired()) {
              throw std::runtime_error("Current group is not referenced by the acquisition.\n");
              // return std::shared_ptr<uff::Group>(nullptr);
            }
            return self.group.lock();
          },
          [](uff::GroupData &self, const std::shared_ptr<uff::Group> &group) {
            self.group = group;
          })
      .def_readwrite("group_timestamp", &uff::GroupData::group_timestamp)
      // .def_readwrite("sequence_timestamps", &uff::GroupData::sequence_timestamps)
      .def_property(
          "sequence_timestamps",
          [](uff::GroupData &self) {
            return py::array_t<double>(
                py::buffer_info(self.sequence_timestamps.data(), sizeof(double),
                                py::format_descriptor<double>::format(), 1,
                                {self.sequence_timestamps.size()}, {sizeof(double)}),
                py::cast(&self.sequence_timestamps));
          },
          [](uff::GroupData &self, const py::buffer &vec) {
            py::buffer_info info = vec.request();
            if (info.item_type_is_equivalent_to<double>()) {
              self.sequence_timestamps = VecFloat64(
                  static_cast<double *>(info.ptr), static_cast<double *>(info.ptr) + info.shape[0]);
            }
          })
      .def_readwrite("event_timestamps", &uff::GroupData::event_timestamps)
      // .def_readwrite("raw_data", &uff::GroupData::raw_data)
      .def_property(
          "raw_data",
          [](uff::GroupData &self) {
            const std::shared_ptr<uff::Group> shared_group = self.group.lock();
            const bool are_data_complex =
                shared_group->sampling_type == uff::Group::SamplingType::IQ;
            const py::ssize_t data_size = self.raw_data.size;
            uff::GroupHelper group_helper{*shared_group};
            void *data_ptr = self.raw_data.buffer.get();
            const py::ssize_t sizeof_data_type_var = group_helper.sizeof_data_type();
            const std::string data_format = group_helper.py_get_format();

            auto buffer = py::buffer_info(
                data_ptr, sizeof_data_type_var, data_format, are_data_complex ? 2 : 1,
                are_data_complex ? std::vector<py::ssize_t>{data_size, 2}
                                 : std::vector<py::ssize_t>{data_size},
                are_data_complex
                    ? std::vector<py::ssize_t>{sizeof_data_type_var * 2, sizeof_data_type_var}
                    : std::vector<py::ssize_t>{sizeof_data_type_var});

            return py::array(buffer, py::cast(&self.raw_data));
          },
          [](uff::GroupData &self, const py::buffer &vec) {
            py::buffer_info info = vec.request();
            if (info.ndim > 2)
              throw std::runtime_error("Dimension error: Too many dimensions in this data array");

            if (info.ndim == 2 && info.shape[1] != 2)
              throw std::runtime_error("Dimension error: Too many data in second dimension");

            auto fake_deleter = [](void *) {};

            self.raw_data.buffer = std::shared_ptr<void>(info.ptr, fake_deleter);
            self.raw_data.size = info.shape[0];
          });

  py::class_<uff::Version>(m, "Version")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("major", &uff::Version::major)
      .def_readwrite("minor", &uff::Version::minor)
      .def_readwrite("patch", &uff::Version::patch);

  py::class_<uff::Dataset>(m, "Dataset")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("version", &uff::Dataset::version)
      .def_readwrite("acquisition", &uff::Dataset::acquisition);

  py::class_<uff::Acquisition>(m, "Acquisition")
      .def(py::init())
      .def(pybind11::self == pybind11::self)
      .def(pybind11::self != pybind11::self)
      .def_readwrite("system", &uff::Acquisition::system)
      .def_readwrite("sound_speed", &uff::Acquisition::sound_speed)
      .def_readwrite("timestamp", &uff::Acquisition::timestamp)
      .def_readwrite("groups", &uff::Acquisition::groups)
      .def_readwrite("group_data", &uff::Acquisition::groups_data);
}
// NOLINTEND(misc-redundant-expression)
