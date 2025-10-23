#pragma once

#include <memory>
#include <optional>

#include <catch2/catch_test_macros.hpp>

#include <urx/detail/compare.h>
#include <urx/utils/detail/clone.h>

namespace urx::utils::test {

template <typename T>
void genericCloneTest(T var, bool smart_comparison_equal = false) {
  auto var_cloned = utils::detail::clone(var);
  REQUIRE(var == var_cloned);
  REQUIRE(std::is_same_v<decltype(var), decltype(var_cloned)>);
  if constexpr (std::is_pointer_v<T>) {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access, cppcoreguidelines-owning-memory)
    delete var_cloned;
  }

  // Raw PTR
  auto* var_ptr_cloned = utils::detail::clone(&var);
  REQUIRE(var == *var_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(&var), decltype(var_ptr_cloned)>);
  // NOLINTNEXTLINE(bugprone-unchecked-optional-access, cppcoreguidelines-owning-memory)
  delete var_ptr_cloned;

  // Shared PTR
  std::shared_ptr<T> var_shared_ptr = std::make_shared<T>(var);
  auto var_shared_ptr_cloned = utils::detail::clone(var_shared_ptr);
  REQUIRE(*var_shared_ptr == *var_shared_ptr_cloned);
  REQUIRE(var_shared_ptr.get() != var_shared_ptr_cloned.get());
  if (smart_comparison_equal) {
    REQUIRE(valueComparison(var_shared_ptr, var_shared_ptr_cloned));
  } else {
    REQUIRE(var_shared_ptr != var_shared_ptr_cloned);
  }
  REQUIRE(std::is_same_v<decltype(var_shared_ptr), decltype(var_shared_ptr_cloned)>);

  // Weak PTR
  std::weak_ptr<T> var_weak_ptr = var_shared_ptr;
  auto var_weak_ptr_cloned = utils::detail::clone(var_weak_ptr);
  REQUIRE(*var_weak_ptr.lock() == *var_weak_ptr_cloned.lock());
  REQUIRE(std::is_same_v<decltype(var_weak_ptr), decltype(var_weak_ptr_cloned)>);

  // Optional
  std::optional<T> var_opt = var;
  auto var_opt_cloned = utils::detail::clone(var_opt);
  REQUIRE(var_opt == var_opt_cloned);
  REQUIRE(var_opt.has_value() == var_opt_cloned.has_value());
  if (var_opt.has_value() && var_opt_cloned.has_value()) {
    REQUIRE(var_opt.value() == var_opt_cloned.value());
  }
  REQUIRE(std::is_same_v<decltype(var_opt), decltype(var_opt_cloned)>);

  // Shared PTR nullptr
  var_shared_ptr = nullptr;
  var_shared_ptr_cloned = utils::detail::clone(var_shared_ptr);
  REQUIRE(var_shared_ptr == var_shared_ptr_cloned);
  REQUIRE(var_shared_ptr.get() == var_shared_ptr_cloned.get());

  // Weak PTR expired
  var_weak_ptr = std::weak_ptr<T>();
  var_weak_ptr_cloned = utils::detail::clone(var_weak_ptr);
  REQUIRE(var_weak_ptr.expired());
  REQUIRE(var_weak_ptr_cloned.expired());

  // Optional nullopt
  var_opt = std::nullopt;
  var_opt_cloned = utils::detail::clone(var_opt);
  REQUIRE(var_opt == var_opt_cloned);
  REQUIRE(!var_opt.has_value());
  REQUIRE(!var_opt_cloned.has_value());
  REQUIRE(std::is_same_v<decltype(var_opt), decltype(var_opt_cloned)>);
}

}  // namespace urx::utils::test