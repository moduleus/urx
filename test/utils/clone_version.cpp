#include <memory>

#include <catch2/catch_test_macros.hpp>

#include <urx/utils/clone.h>

namespace urx::utils::test {

TEST_CASE("Clone Version", "[Clone]") {
  Version vers = {1, 2, 3};
  auto vers_cloned = utils::clone(vers);
  REQUIRE(vers == vers_cloned);
  REQUIRE(std::is_same_v<decltype(vers), decltype(vers_cloned)>);

  // Raw PTR
  auto vers_ptr_cloned = utils::clone(&vers);
  REQUIRE(vers == *vers_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(&vers), decltype(vers_ptr_cloned)>);

  // Shared PTR
  std::shared_ptr<Version> vers_shared_ptr = std::make_shared<Version>(vers);
  auto vers_shared_ptr_cloned = utils::clone(vers_shared_ptr);
  REQUIRE(*vers_shared_ptr == *vers_shared_ptr_cloned);
  REQUIRE(vers_shared_ptr.get() != vers_shared_ptr_cloned.get());
  REQUIRE(vers_shared_ptr != vers_shared_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(vers_shared_ptr), decltype(vers_shared_ptr_cloned)>);

  // Weak PTR
  std::weak_ptr<Version> vers_weak_ptr = vers_shared_ptr;
  auto vers_weak_ptr_cloned = utils::clone(vers_weak_ptr);
  REQUIRE(*vers_weak_ptr.lock() == *vers_weak_ptr_cloned.lock());
  // REQUIRE(vers_weak_ptr == vers_weak_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(vers_weak_ptr), decltype(vers_weak_ptr_cloned)>);

  // Optional
  std::optional<Version> vers_opt = vers;
  auto vers_opt_cloned = utils::clone(vers_opt);
  REQUIRE(vers_opt == vers_opt_cloned);
  REQUIRE(vers_opt.value() == vers_opt_cloned.value());
  REQUIRE(std::is_same_v<decltype(vers_opt), decltype(vers_opt_cloned)>);

  // Shared PTR nullptr
  vers_shared_ptr = nullptr;
  vers_shared_ptr_cloned = utils::clone(vers_shared_ptr);
  REQUIRE(vers_shared_ptr == vers_shared_ptr_cloned);
  REQUIRE(vers_shared_ptr.get() == vers_shared_ptr_cloned.get());

  // Weak PTR expired
  vers_weak_ptr = std::weak_ptr<Version>();
  vers_weak_ptr_cloned = utils::clone(vers_weak_ptr);
  REQUIRE(vers_weak_ptr.expired());
  REQUIRE(vers_weak_ptr_cloned.expired());

  // Optional nullopt
  vers_opt = std::nullopt;
  vers_opt_cloned = utils::clone(vers_opt);
  REQUIRE(vers_opt == vers_opt_cloned);
  REQUIRE(!vers_opt.has_value());
  REQUIRE(!vers_opt_cloned.has_value());
  REQUIRE(std::is_same_v<decltype(vers_opt), decltype(vers_opt_cloned)>);
}

}  // namespace urx::utils::test