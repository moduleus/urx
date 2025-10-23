#pragma once

#include <stdexcept>

#include <H5Cpp.h>

#include <urx/enums.h>

namespace urx::utils::io::detail {

inline urx::DataType h5PredTypeToDataType(const H5::DataType& pred_type) {
  if (pred_type == H5::PredType::NATIVE_INT16) {
    return urx::DataType::INT16;
  }
  if (pred_type == H5::PredType::NATIVE_INT32) {
    return urx::DataType::INT32;
  }
  if (pred_type == H5::PredType::NATIVE_FLOAT) {
    return urx::DataType::FLOAT;
  }
  if (pred_type == H5::PredType::NATIVE_DOUBLE) {
    return urx::DataType::DOUBLE;
  }

  throw std::runtime_error(__FUNCTION__);
}

inline const H5::PredType& dataTypeToH5PredType(urx::DataType data_type) {
  if (data_type == urx::DataType::INT16) {
    return H5::PredType::NATIVE_INT16;
  }
  if (data_type == urx::DataType::INT32) {
    return H5::PredType::NATIVE_INT32;
  }
  if (data_type == urx::DataType::FLOAT) {
    return H5::PredType::NATIVE_FLOAT;
  }
  if (data_type == urx::DataType::DOUBLE) {
    return H5::PredType::NATIVE_DOUBLE;
  }

  throw std::runtime_error(__FUNCTION__);
}

}  // namespace urx::utils::io::detail
