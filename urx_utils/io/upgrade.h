#pragma once

#include <memory>

#include <urx/dataset.h>
#include <urx/v0_5/dataset.h>

namespace urx {

class Upgrade {
 public:
  static std::shared_ptr<urx::Dataset> LoadFromFile(const std::string& filename);
};

}  // namespace urx
