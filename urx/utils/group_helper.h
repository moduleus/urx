#pragma once

#include <cstddef>
#include <string>

#include <urx/group.h>

namespace urx::GroupHelper {
size_t sizeof_data_type(const Group::DataType& data_type);
std::string py_get_format(const Group::DataType& data_type);
size_t sizeof_sample(const Group::SamplingType& sampling_type, const Group::DataType& data_type);
};  // namespace urx::GroupHelper
