#pragma once

#include <string>

#include <urx/dataset.h>

namespace urx::utils::io::writer {
void saveToFile(const std::string& filename, const Dataset& dataset);
}  // namespace urx::utils::io::writer