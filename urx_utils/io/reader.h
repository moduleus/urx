#pragma once

#include <memory>
#include <string>

#include <urx/dataset.h>

namespace urx {
class Reader {
 public:
  static std::shared_ptr<Dataset> loadFromFile(const std::string& filename);
};

}  // namespace urx
