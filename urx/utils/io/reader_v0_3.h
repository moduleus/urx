#pragma once

#include <memory>
#include <string>

#include <urx/v0_3/dataset.h>

namespace urx::utils::io::v0_3 {
class Reader {
 public:
  static std::shared_ptr<urx::v0_3::Dataset> loadFromFile(const std::string& filename);
};

}  // namespace urx::utils::io::v0_3
