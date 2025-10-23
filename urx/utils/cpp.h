#pragma once

#include <complex>
#include <memory>
#include <optional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace urx::utils {

/// Check if T is an instantiation of the template `Class`. For example:
/// `IsInstantiation<shared_ptr, T>` is true if `T == shared_ptr<U>` where U can be anything.
template <template <typename...> class Class, typename T>
struct IsInstantiation : std::false_type {};
template <template <typename...> class Class, typename... Us>
struct IsInstantiation<Class, Class<Us...>> : std::true_type {};

/// Check if T is std::shared_ptr<U> where U can be anything
template <typename T>
using IsSharedPtr = IsInstantiation<std::shared_ptr, T>;

/// Check if T is std::weak_ptr<U> where U can be anything
template <typename T>
using IsWeakPtr = IsInstantiation<std::weak_ptr, T>;

/// Check if T is std::optional<U> where U can be anything
template <typename T>
using IsOptional = IsInstantiation<std::optional, T>;

/// Check if T is std::shared_ptr<U> and U* == RawPtr
template <typename T, typename RawPtr, typename = void>
struct IsSharedPtrTo : std::false_type {};

template <typename T, typename RawPtr>
struct IsSharedPtrTo<T, RawPtr, std::void_t<typename T::element_type>>
    : std::conjunction<IsSharedPtr<T>, std::is_same<typename T::element_type*, RawPtr>> {};

enum class ContainerType { RAW, VECTOR, SHARED_PTR, WEAK_PTR, OPTIONAL };

template <typename T>
struct TypeContainer {
  static constexpr ContainerType VALUE = ContainerType::RAW;
};

template <typename T>
struct TypeContainer<std::vector<T>> {
  static constexpr ContainerType VALUE = ContainerType::VECTOR;
};

template <typename T>
struct TypeContainer<std::shared_ptr<T>> {
  static constexpr ContainerType VALUE = ContainerType::SHARED_PTR;
};

template <typename T>
struct TypeContainer<std::weak_ptr<T>> {
  static constexpr ContainerType VALUE = ContainerType::WEAK_PTR;
};

template <typename T>
struct TypeContainer<std::optional<T>> {
  static constexpr ContainerType VALUE = ContainerType::OPTIONAL;
};

template <typename T>
struct IsComplex : public std::false_type {};

template <typename T>
struct IsComplex<std::complex<T>> : public std::true_type {};

template <typename T>
struct IsComplex<std::vector<std::complex<T>>> : public std::true_type {};

template <typename T>
std::type_index nameTypeid() {
  return typeid(T);
}

template <class... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

using MapToSharedPtr = std::unordered_map<std::type_index, const void*>;

template <typename T>
const std::vector<std::shared_ptr<T>>& getSharedPtr(const MapToSharedPtr& map) {
  return *reinterpret_cast<const std::vector<std::shared_ptr<T>>*>(map.at(nameTypeid<T>()));
}

}  // namespace urx::utils