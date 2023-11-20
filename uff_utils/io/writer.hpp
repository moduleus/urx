#pragma once

#include <string_view>

#include <uff/dataset.h>

namespace uff {
class Writer {
 public:
  static void saveToFile(const std::string& filename, const Dataset& dataset);
};

}  // namespace uff