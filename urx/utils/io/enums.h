#pragma once

#include <memory>
#include <string>

#include <urx/dataset.h>

namespace urx::utils::io::enums {
template <typename T>
T stringToEnum(const std::string& enumeration);
template <typename T>
std::string enumToString(T enumeration);
}  // namespace urx::utils::io::enums
