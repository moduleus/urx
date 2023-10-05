#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <algorithm>  // for std::ranges::copy depending on lib support
#include <complex>
#include <iostream>
#include <iterator>  // for std::ostream_iterator
#include <vector>

#include <uff/group_data.h>

namespace py = pybind11;

using VecInt16 = std::vector<int16_t>;
using VecInt32 = std::vector<int32_t>;
using VecFloat32 = std::vector<float>;
using VecFloat64 = std::vector<double>;
using VecCompInt16 = std::vector<std::complex<int16_t>>;
using VecCompInt32 = std::vector<std::complex<int32_t>>;
using VecCompFloat32 = std::vector<std::complex<float>>;
using VecCompFloat64 = std::vector<std::complex<double>>;

PYBIND11_MAKE_OPAQUE(VecInt16);
PYBIND11_MAKE_OPAQUE(VecInt32);
PYBIND11_MAKE_OPAQUE(VecFloat32);
PYBIND11_MAKE_OPAQUE(VecFloat64);
PYBIND11_MAKE_OPAQUE(VecCompInt16);
PYBIND11_MAKE_OPAQUE(VecCompInt32);
PYBIND11_MAKE_OPAQUE(VecCompFloat32);
PYBIND11_MAKE_OPAQUE(VecCompFloat64);

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  if (!v.empty()) {
    std::ranges::copy(v, std::ostream_iterator<T>(out, "\n"));
  }
  return out;
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
        if (info.format != py::format_descriptor<int16_t>::format() &&
            info.format != py::format_descriptor<int32_t>::format() &&
            info.format != py::format_descriptor<std::complex<float>>::format() &&
            info.format != py::format_descriptor<std::complex<double>>::format())
          throw std::runtime_error(
              "Incompatible format: expected a int16 array of pair, int32 array of pair, complex64 array or complex128 array!");

        if ((info.format == py::format_descriptor<int16_t>::format() ||
             info.format == py::format_descriptor<int32_t>::format()) &&
            (info.ndim != 2 || info.shape[1] != 2))
          throw std::runtime_error("Incompatible buffer dimension!");

        if ((info.format == py::format_descriptor<std::complex<float>>::format() ||
             info.format == py::format_descriptor<std::complex<double>>::format()) &&
            (info.ndim != 1))
          throw std::runtime_error("Incompatible buffer dimension!");

        if (info.format == py::format_descriptor<int16_t>::format()) {
          auto ptr = static_cast<std::complex<int16_t> *>(info.ptr);
          VecCompInt16 vec;
          vec.reserve(info.shape[0]);
          vec.insert(vec.begin(), ptr, ptr + info.shape[0]);
          return vec;
        } else if (info.format == py::format_descriptor<int32_t>::format()) {
          auto ptr = static_cast<std::complex<int32_t> *>(info.ptr);
          VecCompInt16 vec;
          vec.resize(info.shape[0]);
          for (size_t i = 0; i < info.shape[0]; ++i) {
            vec[i] = static_cast<std::complex<int32_t>>(ptr[i]);
          }
          return vec;
        } else if (info.format == py::format_descriptor<std::complex<float>>::format()) {
          auto ptr = static_cast<std::complex<float> *>(info.ptr);
          VecCompInt16 vec;
          vec.resize(info.shape[0]);
          for (size_t i = 0; i < info.shape[0]; ++i) {
            vec[i] = static_cast<std::complex<int16_t>>(ptr[i]);
          }
          return vec;
        } else if (info.format == py::format_descriptor<std::complex<double>>::format()) {
          auto ptr = static_cast<std::complex<double> *>(info.ptr);
          auto vec = VecCompInt16(info.shape[0]);
          for (size_t i = 0; i < info.shape[0]; ++i) {
            vec[i] = static_cast<std::complex<int16_t>>(ptr[i]);
          }
          return vec;
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
        if (info.format != py::format_descriptor<int16_t>::format() &&
            info.format != py::format_descriptor<int32_t>::format() &&
            info.format != py::format_descriptor<std::complex<float>>::format() &&
            info.format != py::format_descriptor<std::complex<double>>::format())
          throw std::runtime_error(
              "Incompatible format: expected a int16 array of pair, int32 array of pair, complex64 array or complex128 array!");

        if ((info.format == py::format_descriptor<int16_t>::format() ||
             info.format == py::format_descriptor<int32_t>::format()) &&
            (info.ndim != 2 || info.shape[1] != 2))
          throw std::runtime_error("Incompatible buffer dimension!");

        if ((info.format == py::format_descriptor<std::complex<float>>::format() ||
             info.format == py::format_descriptor<std::complex<double>>::format()) &&
            (info.ndim != 1))
          throw std::runtime_error("Incompatible buffer dimension!");

        if (info.format == py::format_descriptor<int32_t>::format()) {
          auto ptr = static_cast<std::complex<int32_t> *>(info.ptr);
          VecCompInt32 vec;
          vec.reserve(info.shape[0]);
          vec.insert(vec.begin(), ptr, ptr + info.shape[0]);
          return vec;
        } else if (info.format == py::format_descriptor<int16_t>::format()) {
          auto ptr = static_cast<std::complex<int16_t> *>(info.ptr);
          VecCompInt32 vec;
          vec.resize(info.shape[0]);
          for (size_t i = 0; i < info.shape[0]; ++i) {
            vec[i] = static_cast<std::complex<int32_t>>(ptr[i]);
          }
          return vec;
        } else if (info.format == py::format_descriptor<std::complex<float>>::format()) {
          auto ptr = static_cast<std::complex<float> *>(info.ptr);
          VecCompInt32 vec;
          vec.resize(info.shape[0]);
          for (size_t i = 0; i < info.shape[0]; ++i) {
            vec[i] = static_cast<std::complex<int32_t>>(ptr[i]);
          }
          return vec;
        } else if (info.format == py::format_descriptor<std::complex<double>>::format()) {
          auto ptr = static_cast<std::complex<double> *>(info.ptr);
          auto vec = VecCompInt32(info.shape[0]);
          for (size_t i = 0; i < info.shape[0]; ++i) {
            vec[i] = static_cast<std::complex<int32_t>>(ptr[i]);
          }
          return vec;
        }
      }));

  py::bind_vector<VecInt16>(m, "VecInt16", py::buffer_protocol());
  py::bind_vector<VecInt32>(m, "VecInt32", py::buffer_protocol());
  py::bind_vector<VecFloat32>(m, "VecFloat32", py::buffer_protocol());
  py::bind_vector<VecFloat64>(m, "VecFloat64", py::buffer_protocol());
  py::bind_vector<VecCompFloat32>(m, "VecCompFloat32", py::buffer_protocol());
  py::bind_vector<VecCompFloat64>(m, "VecCompFloat64", py::buffer_protocol());

  py::class_<uff::GroupData>(m, "GroupData")
      .def(py::init())
      .def("printData", &uff::GroupData::printData)
      .def("checkInt16Type", &uff::GroupData::checkType<std::vector<int16_t>>)
      .def("checkInt32Type", &uff::GroupData::checkType<std::vector<int32_t>>)
      .def("checkFloat32Type", &uff::GroupData::checkType<std::vector<float>>)
      .def("checkFloat64Type", &uff::GroupData::checkType<std::vector<double>>)
      .def("checkComplexInt16Type", &uff::GroupData::checkType<std::vector<std::complex<int16_t>>>)
      .def("checkComplexInt32Type", &uff::GroupData::checkType<std::vector<std::complex<int32_t>>>)
      .def("checkComplexFloat32Type", &uff::GroupData::checkType<std::vector<std::complex<float>>>)
      .def("checkComplexFloat64Type", &uff::GroupData::checkType<std::vector<std::complex<double>>>)
      .def_readwrite("raw_data", &uff::GroupData::raw_data);
}