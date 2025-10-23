#include "clone.h"

#include <urx/dataset.h>
#include <urx/utils/detail/clone.h>

namespace urx::utils {

Dataset clone(const Dataset& value) { return detail::clone<Dataset>(value); }

std::shared_ptr<Dataset> clone(const std::shared_ptr<Dataset>& value) {
  return detail::clone<std::shared_ptr<Dataset>>(value);
}

}  // namespace urx::utils
