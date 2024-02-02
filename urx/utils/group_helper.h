#pragma once

#include <cstddef>
#include <string>

#include <urx/group.h>

namespace urx::utils::GroupHelper {
size_t sizeof_data_type(const DataType& data_type);
std::string py_get_format(const DataType& data_type);
size_t sizeof_sample(const SamplingType& sampling_type, const DataType& data_type);
};  // namespace urx::utils::GroupHelper
