#pragma once

#include <memory>
#include <string>

#include <uff/dataset.h>

namespace uff {
class Reader {
 public:
  static std::shared_ptr<Dataset> loadFromFile(const std::string& filename);
};

}  // namespace uff
