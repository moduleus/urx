#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include <H5Cpp.h>

#include <urx/utils/cpp.h>
#include <urx/utils/export.h>
#include <urx/utils/serialize_helper.h>

namespace urx::utils::io {

URX_UTILS_EXPORT const std::unordered_map<std::type_index, const H5::PredType*>& getStdToHdf5();

constexpr int ITER_LENGTH = 8;
URX_UTILS_EXPORT extern bool use_attribute;

using AllTypeInVariant [[deprecated("Will be move to urx::utils namespace.")]] =
    urx::utils::AllTypeInVariant;

[[deprecated("Will be move to urx::utils namespace.")]]
URX_UTILS_EXPORT const std::unordered_map<
    std::type_index, std::vector<std::pair<urx::utils::AllTypeInVariant, std::string>>>&
getMemberMap();

using MapToSharedPtr [[deprecated("Will be move to urx::utils namespace.")]] =
    urx::utils::MapToSharedPtr;

template <typename T>
[[deprecated("Will be move to urx::utils namespace.")]]
std::type_index nameTypeid() {
  return urx::utils::nameTypeid<T>();
}

template <typename T>
[[deprecated("Will be move to urx::utils namespace.")]]
const std::vector<std::shared_ptr<T>>& getSharedPtr(const urx::utils::MapToSharedPtr& map) {
  return urx::utils::getSharedPtr<T>(map);
}

template <class... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

}  // namespace urx::utils::io
