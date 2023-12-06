#pragma once

#include <memory>
#include <string>

#include <urx/v0_3/dataset.h>

namespace urx::v0_3 {
class Reader {
 public:
  static std::shared_ptr<Dataset> loadFromFile(const std::string& filename);
};

}  // namespace urx::v0_3
