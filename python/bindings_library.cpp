#include <algorithm>
#include <complex>
#include <cstdint>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <pybind11/buffer_info.h>
#include <pybind11/cast.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>       // IWYU pragma: keep
#include <pybind11/stl_bind.h>  // IWYU pragma: keep

#include <urx/detail/compare.h>
#include <urx/detail/raw_data.h>
#include <urx/enums.h>
#include <urx/python/bindings.h>
#include <urx/python/utils/group_helper.h>
#include <urx/utils/group_helper.h>

namespace urx::python::detail {

std::shared_ptr<urx::RawData> pyArrayToRawData(const py::array& array) {
  py::buffer_info info = array.request();
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
      return std::make_shared<urx::python::RawDataWeakPython<std::complex<double>>>(info.ptr,
                                                                                    info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<std::complex<float>>()) {
      return std::make_shared<urx::python::RawDataWeakPython<std::complex<float>>>(info.ptr,
                                                                                   info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<int16_t>()) {
      return std::make_shared<urx::python::RawDataWeakPython<int16_t>>(info.ptr, info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<int32_t>()) {
      return std::make_shared<urx::python::RawDataWeakPython<int32_t>>(info.ptr, info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<float>()) {
      return std::make_shared<urx::python::RawDataWeakPython<float>>(info.ptr, info.shape[0]);
    }
    if (info.item_type_is_equivalent_to<double>()) {
      return std::make_shared<urx::python::RawDataWeakPython<double>>(info.ptr, info.shape[0]);
    }
    throw std::runtime_error("No equivalent data type to provided buffer");
  }
  // info.ndim == 2
  if (info.item_type_is_equivalent_to<int16_t>()) {
    return std::make_shared<urx::python::RawDataWeakPython<std::complex<int16_t>>>(info.ptr,
                                                                                   info.shape[0]);
  }
  if (info.item_type_is_equivalent_to<int32_t>()) {
    return std::make_shared<urx::python::RawDataWeakPython<std::complex<int32_t>>>(info.ptr,
                                                                                   info.shape[0]);
  }
  if (info.item_type_is_equivalent_to<float>()) {
    return std::make_shared<urx::python::RawDataWeakPython<std::complex<float>>>(info.ptr,
                                                                                 info.shape[0]);
  }
  if (info.item_type_is_equivalent_to<double>()) {
    return std::make_shared<urx::python::RawDataWeakPython<std::complex<double>>>(info.ptr,
                                                                                  info.shape[0]);
  }
  throw std::runtime_error("No equivalent data type to provided buffer");
}

}  // namespace urx::python::detail
