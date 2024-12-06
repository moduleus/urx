#pragma once

#include <complex>
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
constexpr bool is_complex() {
  return IsComplex<T>::value;
}

template <typename T>
constexpr bool is_complex(const T&) {
  return is_complex<T>();
}

}  // namespace urx::utils