#pragma once

#include <cstddef>
#include <string>

#include "urx/enums.h"

namespace urx::utils::group_helper {
size_t sizeofDataType(const DataType& data_type);
std::string pyGetFormat(const DataType& data_type);
size_t sizeofSample(const SamplingType& sampling_type, const DataType& data_type);
};  // namespace urx::utils::group_helper
