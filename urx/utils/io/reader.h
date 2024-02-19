#pragma once

#include <memory>
#include <string>

#include <urx/dataset.h>
#include <urx/excitation.h>
#include <urx/group.h>

namespace urx::utils::io::reader {
std::shared_ptr<Dataset> loadFromFile(const std::string& filename);
}  // namespace urx::utils::io::reader
