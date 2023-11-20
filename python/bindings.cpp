#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <algorithm>  // for std::ranges::copy depending on lib support
#include <complex>
#include <iostream>
#include <iterator>  // for std::ostream_iterator
#include <variant>
#include <vector>

#include <uff/acquisition.h>
#include <uff/dataset.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/version.h>

namespace py = pybind11;

using VecInt16 = std::vector<int16_t>;
using VecInt32 = std::vector<int32_t>;
using VecFloat32 = std::vector<float>;
using VecFloat64 = std::vector<double>;
using VecCompInt16 = std::vector<std::complex<int16_t>>;
using VecCompInt32 = std::vector<std::complex<int32_t>>;
using VecCompFloat32 = std::vector<std::complex<float>>;
using VecCompFloat64 = std::vector<std::complex<double>>;

using VecGroup = std::vector<std::shared_ptr<uff::Group>>;

PYBIND11_MAKE_OPAQUE(VecInt16);
PYBIND11_MAKE_OPAQUE(VecInt32);
PYBIND11_MAKE_OPAQUE(VecFloat32);
PYBIND11_MAKE_OPAQUE(VecFloat64);
PYBIND11_MAKE_OPAQUE(VecCompInt16);
PYBIND11_MAKE_OPAQUE(VecCompInt32);
PYBIND11_MAKE_OPAQUE(VecCompFloat32);
PYBIND11_MAKE_OPAQUE(VecCompFloat64);

PYBIND11_MAKE_OPAQUE(VecGroup);

template <typename VecDataType>
bool checkType(uff::GroupData &vec) {
  if (auto d = std::get_if<VecDataType>(&vec.raw_data)) {
    return true;
  }
  return false;
}

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

PYBIND11_MODULE(bindings, m) {
  m.doc() = "Variant C++ binding POC";

  py::class_<VecCompInt16>(m, "VecCompInt16", py::buffer_protocol())
      .def_buffer([](VecCompInt16 &v) -> py::buffer_info {
        return py::buffer_info(v.data(),                                 /* Pointer to buffer */
                               sizeof(int16_t),                          /* Size of one scalar */
                               py::format_descriptor<int16_t>::format(), /* Python struct-style
                                                           format descriptor */
                               2,                                        /* Number of dimensions */
                               {size_t(v.size()), size_t(2)},            /* Buffer dimensions */
                               {sizeof(int16_t) * size_t(2), /* Strides (in bytes) for each index */
                                sizeof(int16_t)});
      })
      .def("__getitem__",
           [](VecCompInt16 &v, py::ssize_t i) {
             if (i >= v.size()) {
               throw py::index_error();
             }
             return v[i];
           })
      .def("__getitem__",
           [](VecCompInt16 &v, std::pair<py::ssize_t, py::ssize_t> i) {
             if (i.first >= v.size() || i.second >= 2) {
               throw py::index_error();
             }
             if (i.second == 0) {
               return v[i.first].real();
             } else {
               return v[i.first].imag();
             }
           })
      .def("__setitem__",
           [](VecCompInt16 &v, py::ssize_t i, std::pair<int16_t, int16_t> p) {
             if (i >= v.size()) {
               throw py::index_error();
             }
             v[i] = std::complex<int16_t>(p.first, p.second);
           })
      .def("__setitem__",
           [](VecCompInt16 &v, std::pair<py::ssize_t, py::ssize_t> i, int16_t value) {
             if (i.first >= v.size() || i.second >= 2) {
               throw py::index_error();
             }
             if (i.second == 0) {
               v[i.first].real(value);
             } else {
               v[i.first].imag(value);
             }
           })
      .def("__len__", [](VecCompInt16 &v) { return v.size(); })
      .def("__repr__",
           [](const VecCompInt16 &v) {
             std::ostringstream stream;
             stream << v;
             return stream.str();
           })
      .def(py::init([](py::buffer b) {
        /* Request a buffer descriptor from Python */
        py::buffer_info info = b.request();

        /* Some basic validation checks ... */
        if ((info.item_type_is_equivalent_to<int16_t>() ||
             info.item_type_is_equivalent_to<int32_t>()) &&
            (info.ndim != 2 || info.shape[1] != 2))
          throw std::runtime_error("Incompatible buffer dimension!");

        if ((info.item_type_is_equivalent_to<std::complex<float>>() ||
             info.item_type_is_equivalent_to<std::complex<double>>()) &&
            (info.ndim != 1))
          throw std::runtime_error("Incompatible buffer dimension!");

        if (info.item_type_is_equivalent_to<int16_t>()) {
          auto ptr = static_cast<std::complex<int16_t> *>(info.ptr);
          VecCompInt16 vec;
          vec.reserve(info.shape[0]);
          vec.insert(vec.begin(), ptr, ptr + info.shape[0]);
          return vec;
        } else if (info.item_type_is_equivalent_to<int32_t>()) {
          auto ptr = static_cast<std::complex<int32_t> *>(info.ptr);
          VecCompInt16 vec;
          vec.reserve(info.shape[0]);
          for (auto i = 0; i < info.shape[0]; ++i) {
            vec.push_back(static_cast<std::complex<int16_t>>(ptr[i]));
          }
          return vec;
        } else if (info.item_type_is_equivalent_to<std::complex<float>>()) {
          auto ptr = static_cast<std::complex<float> *>(info.ptr);
          VecCompInt16 vec;
          vec.reserve(info.shape[0]);
          for (auto i = 0; i < info.shape[0]; ++i) {
            vec.push_back(static_cast<std::complex<int16_t>>(ptr[i]));
          }
          return vec;
        } else if (info.item_type_is_equivalent_to<std::complex<double>>()) {
          auto ptr = static_cast<std::complex<double> *>(info.ptr);
          VecCompInt16 vec;
          vec.reserve(info.shape[0]);
          for (auto i = 0; i < info.shape[0]; ++i) {
            vec.push_back(static_cast<std::complex<int16_t>>(ptr[i]));
          }
          return vec;
        } else {
          throw std::runtime_error(
              "Incompatible format: expected a int16 array of pair, int32 array of pair, complex64 "
              "array or complex128 array!");
        }
      }));

  py::class_<VecCompInt32>(m, "VecCompInt32", py::buffer_protocol())
      .def_buffer([](VecCompInt32 &v) -> py::buffer_info {
        return py::buffer_info(v.data(),                                 /* Pointer to buffer */
                               sizeof(int32_t),                          /* Size of one scalar */
                               py::format_descriptor<int32_t>::format(), /* Python struct-style
                                                           format descriptor */
                               2,                                        /* Number of dimensions */
                               {size_t(v.size()), size_t(2)},            /* Buffer dimensions */
                               {sizeof(int32_t) * size_t(2), /* Strides (in bytes) for each index */
                                sizeof(int32_t)});
      })
      .def("__getitem__",
           [](VecCompInt32 &v, py::ssize_t i) {
             if (i >= v.size()) {
               throw py::index_error();
             }
             return v[i];
           })
      .def("__getitem__",
           [](VecCompInt32 &v, std::pair<py::ssize_t, py::ssize_t> i) {
             if (i.first >= v.size() || i.second >= 2) {
               throw py::index_error();
             }
             if (i.second == 0) {
               return v[i.first].real();
             } else {
               return v[i.first].imag();
             }
           })
      .def("__setitem__",
           [](VecCompInt32 &v, py::ssize_t i, std::pair<int32_t, int32_t> p) {
             if (i >= v.size()) {
               throw py::index_error();
             }
             v[i] = std::complex<int32_t>(p.first, p.second);
           })
      .def("__setitem__",
           [](VecCompInt32 &v, std::pair<py::ssize_t, py::ssize_t> i, int32_t value) {
             if (i.first >= v.size() || i.second >= 2) {
               throw py::index_error();
             }
             if (i.second == 0) {
               v[i.first].real(value);
             } else {
               v[i.first].imag(value);
             }
           })
      .def("__len__", [](VecCompInt32 &v) { return v.size(); })
      .def("__repr__",
           [](const VecCompInt32 &v) {
             std::ostringstream stream;
             stream << v;
             return stream.str();
           })
      .def(py::init([](py::buffer b) {
        /* Request a buffer descriptor from Python */
        py::buffer_info info = b.request();

        /* Some basic validation checks ... */
        if ((info.item_type_is_equivalent_to<int16_t>() ||
             info.item_type_is_equivalent_to<int32_t>()) &&
            (info.ndim != 2 || info.shape[1] != 2))
          throw std::runtime_error("Incompatible buffer dimension!");

        if ((info.item_type_is_equivalent_to<std::complex<float>>() ||
             info.item_type_is_equivalent_to<std::complex<double>>()) &&
            (info.ndim != 1))
          throw std::runtime_error("Incompatible buffer dimension!");

        if (info.item_type_is_equivalent_to<int32_t>()) {
          auto ptr = static_cast<std::complex<int32_t> *>(info.ptr);
          VecCompInt32 vec;
          vec.reserve(info.shape[0]);
          vec.insert(vec.begin(), ptr, ptr + info.shape[0]);
          return vec;
        } else if (info.item_type_is_equivalent_to<int16_t>()) {
          auto ptr = static_cast<std::complex<int16_t> *>(info.ptr);
          VecCompInt32 vec;
          vec.reserve(info.shape[0]);
          for (auto i = 0; i < info.shape[0]; ++i) {
            vec.push_back(static_cast<std::complex<int32_t>>(ptr[i]));
          }
          return vec;
        } else if (info.item_type_is_equivalent_to<std::complex<float>>()) {
          auto ptr = static_cast<std::complex<float> *>(info.ptr);
          VecCompInt32 vec;
          vec.reserve(info.shape[0]);
          for (auto i = 0; i < info.shape[0]; ++i) {
            vec.push_back(static_cast<std::complex<int32_t>>(ptr[i]));
          }
          return vec;
        } else if (info.item_type_is_equivalent_to<std::complex<double>>()) {
          auto ptr = static_cast<std::complex<double> *>(info.ptr);
          VecCompInt32 vec;
          vec.reserve(info.shape[0]);
          for (auto i = 0; i < info.shape[0]; ++i) {
            vec.push_back(static_cast<std::complex<int32_t>>(ptr[i]));
          }
          return vec;
        } else {
          throw std::runtime_error(
              "Incompatible format: expected a int16 array of pair, int32 array of pair, complex64 "
              "array or complex128 array!");
        }
      }));

  py::bind_vector<VecInt16>(m, "VecInt16", py::buffer_protocol());
  py::bind_vector<VecInt32>(m, "VecInt32", py::buffer_protocol());
  py::bind_vector<VecFloat32>(m, "VecFloat32", py::buffer_protocol());
  py::bind_vector<VecFloat64>(m, "VecFloat64", py::buffer_protocol());
  py::bind_vector<VecCompFloat32>(m, "VecCompFloat32", py::buffer_protocol());
  py::bind_vector<VecCompFloat64>(m, "VecCompFloat64", py::buffer_protocol());

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
          [](uff::GroupData &self, py::kwargs kwargs) {
            if (self.group.expired()) {
              throw std::runtime_error("Current group is not referenced by the acquisition.\n");
              // return std::shared_ptr<uff::Group>(nullptr);
            } else {
              return self.group.lock();
            }
          },
          [](uff::GroupData &self, std::shared_ptr<uff::Group> group, py::kwargs kwargs) {
            self.group = group;
          })
      .def_readwrite("group_timestamp", &uff::GroupData::group_timestamp)
      // .def_readwrite("sequence_timestamps", &uff::GroupData::sequence_timestamps)
      .def_property(
          "sequence_timestamps",
          [](uff::GroupData &self, py::kwargs kwargs) {
            return py::array_t<double>(
                py::buffer_info(self.sequence_timestamps.data(), /* Pointer to buffer */
                                sizeof(double),                  /* Size of one scalar */
                                py::format_descriptor<
                                    double>::format(), /* Python struct-style format descriptor */
                                1,                     /* Number of dimensions */
                                {self.sequence_timestamps.size()}, /* Buffer dimensions */
                                {sizeof(double)}),
                py::cast(&self.sequence_timestamps));
          },
          [](uff::GroupData &self, py::buffer vec, py::kwargs kwargs) {
            py::buffer_info info = vec.request();
            if (info.item_type_is_equivalent_to<double>()) {
              self.sequence_timestamps =
                  VecFloat64((double *)info.ptr, (double *)info.ptr + info.shape[0]);
            }
          })
      .def_readwrite("event_timestamps", &uff::GroupData::event_timestamps)
      // .def_readwrite("raw_data", &uff::GroupData::raw_data)
      .def_property(
          "raw_data",
          [](uff::GroupData &self, py::kwargs kwargs) {
            const bool are_data_complex =
                std::visit([](auto &&vec) { return is_complex(vec); }, self.raw_data);
            py::ssize_t data_size =
                std::visit([](auto &&vec) { return vec.size(); }, self.raw_data);
            void *data_ptr =
                std::visit([](auto &&vec) { return (void *)vec.data(); }, self.raw_data);
            py::ssize_t sizeof_data_type_var =
                std::visit([](auto &&vec) { return sizeof_data_type(vec); }, self.raw_data);
            std::string data_format =
                std::visit([](auto &&vec) { return get_format(vec); }, self.raw_data);

            auto buffer = py::buffer_info(
                data_ptr,                 /* Pointer to buffer */
                sizeof_data_type_var,     /* Size of one scalar */
                data_format,              /* Python struct-style format descriptor */
                are_data_complex ? 2 : 1, /* Number of dimensions */
                are_data_complex ? std::vector<py::ssize_t>{data_size, py::ssize_t(2)}
                                 : std::vector<py::ssize_t>{data_size}, /* Buffer dimensions */
                are_data_complex
                    ? std::vector<py::ssize_t>{sizeof_data_type_var * 2, sizeof_data_type_var}
                    : std::vector<py::ssize_t>{sizeof_data_type_var});

            return py::array(buffer, py::cast(&self.raw_data));
          },
          [](uff::GroupData &self, py::buffer vec, py::kwargs kwargs) {
            py::buffer_info info = vec.request();
            if (info.ndim > 2)
              throw std::runtime_error("Dimension error: Too many dimensions in this data array");

            if (info.ndim == 2 && info.shape[1] != 2)
              throw std::runtime_error("Dimension error: Too many data in second dimension");

            if (info.item_type_is_equivalent_to<double>()) {
              if (info.ndim == 1)  // RF
                self.raw_data = VecFloat64((double *)info.ptr, (double *)info.ptr + info.shape[0]);
              else  // IQ
                self.raw_data = VecCompFloat64((std::complex<double> *)info.ptr,
                                               (std::complex<double> *)info.ptr + info.shape[0]);
            } else if (info.item_type_is_equivalent_to<float>()) {
              if (info.ndim == 1)  // RF
                self.raw_data = VecFloat32((float *)info.ptr, (float *)info.ptr + info.shape[0]);
              else  // IQ
                self.raw_data = VecCompFloat32((std::complex<float> *)info.ptr,
                                               (std::complex<float> *)info.ptr + info.shape[0]);
            } else if (info.item_type_is_equivalent_to<int32_t>()) {
              if (info.ndim == 1)  // RF
                self.raw_data = VecInt32((int32_t *)info.ptr, (int32_t *)info.ptr + info.shape[0]);
              else  // IQ
                self.raw_data = VecCompInt32((std::complex<int32_t> *)info.ptr,
                                             (std::complex<int32_t> *)info.ptr + info.shape[0]);
            } else if (info.item_type_is_equivalent_to<int16_t>()) {
              if (info.ndim == 1)  // RF
                self.raw_data = VecInt16((int16_t *)info.ptr, (int16_t *)info.ptr + info.shape[0]);
              else  // IQ
                self.raw_data = VecCompInt16((std::complex<int16_t> *)info.ptr,
                                             (std::complex<int16_t> *)info.ptr + info.shape[0]);
            } else if (info.item_type_is_equivalent_to<std::complex<double>>()) {
              self.raw_data = VecCompFloat64((std::complex<double> *)info.ptr,
                                             (std::complex<double> *)info.ptr + info.shape[0]);
            } else if (info.item_type_is_equivalent_to<std::complex<float>>()) {
              self.raw_data = VecCompFloat32((std::complex<float> *)info.ptr,
                                             (std::complex<float> *)info.ptr + info.shape[0]);
            }
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

  m.def("checkInt16Type", &checkType<std::vector<int16_t>>);
  m.def("checkInt32Type", &checkType<std::vector<int32_t>>);
  m.def("checkFloat32Type", &checkType<std::vector<float>>);
  m.def("checkFloat64Type", &checkType<std::vector<double>>);

  m.def("checkComplexInt16Type", &checkType<std::vector<std::complex<int16_t>>>);
  m.def("checkComplexInt32Type", &checkType<std::vector<std::complex<int32_t>>>);
  m.def("checkComplexFloat32Type", &checkType<std::vector<std::complex<float>>>);
  m.def("checkComplexFloat64Type", &checkType<std::vector<std::complex<double>>>);
}