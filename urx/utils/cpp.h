#pragma once

#include <memory>
#include <optional>

namespace urx::utils {

/// Check if T is an instantiation of the template `Class`. For example:
/// `is_instantiation<shared_ptr, T>` is true if `T == shared_ptr<U>` where U can be anything.
template <template <typename...> class Class, typename T>
struct is_instantiation : std::false_type {};
template <template <typename...> class Class, typename... Us>
struct is_instantiation<Class, Class<Us...>> : std::true_type {};

/// Check if T is std::shared_ptr<U> where U can be anything
template <typename T>
using is_shared_ptr = is_instantiation<std::shared_ptr, T>;

/// Check if T is std::weak_ptr<U> where U can be anything
template <typename T>
using is_weak_ptr = is_instantiation<std::weak_ptr, T>;

/// Check if T is std::optional<U> where U can be anything
template <typename T>
using is_optional = is_instantiation<std::optional, T>;

}  // namespace urx::utils