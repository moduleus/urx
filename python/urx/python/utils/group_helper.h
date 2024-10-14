#pragma once

#include <string>

#include <urx/enums.h>
#include <urx/python/export.h>

namespace urx::python::utils {
URX_PYTHON_EXPORT std::string pyGetFormat(const DataType& data_type);
};  // namespace urx::python::utils
