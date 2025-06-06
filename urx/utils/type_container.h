#pragma once

#include <memory>
#include <optional>
#include <vector>

namespace urx::utils {

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

}  // namespace urx::utils
