#pragma once

#include <string>

#include <urx/dataset.h>

namespace urx::utils::io {
class Writer {
 public:
  static void saveToFile(const std::string& filename, const Dataset& dataset);
};

}  // namespace urx::utils::io