#pragma once

#include <H5Cpp.h>

#define H5DataType \
  (std::is_same<DataType, float>::value ? H5::PredType::NATIVE_FLOAT : H5::PredType::NATIVE_SHORT)
#define H5MetadataType \
  (std::is_same<double, float>::value ? H5::PredType::NATIVE_FLOAT : H5::PredType::NATIVE_DOUBLE)
namespace urx {}  // namespace urx