#pragma once

#include <memory>
#include <string>

#include <urx/dataset.h>

namespace urx {

class Upgrade {
 public:
  static std::shared_ptr<urx::Dataset> LoadFromFile(const std::string& filename);
};

}  // namespace urx
