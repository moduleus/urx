#pragma once

#include <string>

#include <urx/dataset.h>
#include <urx/utils/export.h>

namespace urx::utils::io::writer {

URX_UTILS_EXPORT void saveToFile(const std::string& filename, const Dataset& dataset);

}  // namespace urx::utils::io::writer
