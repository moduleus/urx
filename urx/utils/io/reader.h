#pragma once

#include <memory>
#include <string>

#include <urx/dataset.h>
#include <urx/utils/export.h>

namespace urx::utils::io {

URX_UTILS_EXPORT std::shared_ptr<Dataset> loadFromFile(const std::string& filename);

}  // namespace urx::utils::io
