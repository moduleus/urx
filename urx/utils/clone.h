#pragma once

#include <memory>

#include <urx/dataset.h>
#include <urx/utils/export.h>

namespace urx::utils {

URX_UTILS_EXPORT Dataset clone(const Dataset& value);
URX_UTILS_EXPORT std::shared_ptr<Dataset> clone(const std::shared_ptr<Dataset>& value);

}  // namespace urx::utils
