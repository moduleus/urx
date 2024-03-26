#pragma once

#include <cstddef>
#include <string>

#include <urx/enums.h>
#include <urx/utils/export.h>

namespace urx::utils::group_helper {
URX_UTILS_EXPORT size_t sizeofDataType(const DataType& data_type);
URX_UTILS_EXPORT std::string pyGetFormat(const DataType& data_type);
URX_UTILS_EXPORT size_t sizeofSample(const SamplingType& sampling_type, const DataType& data_type);
};  // namespace urx::utils::group_helper
