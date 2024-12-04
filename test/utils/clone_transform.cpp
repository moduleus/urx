#include <memory>

#include <catch2/catch_test_macros.hpp>

#include <urx/utils/clone.h>

namespace urx::utils::test {

TEST_CASE("Clone Transform", "[Clone]") {
  Transform t = {{1, 2, 3}, {4, 5, 6}};
  auto t_cloned = utils::clone(t);
  REQUIRE(t == t_cloned);
  REQUIRE(std::is_same_v<decltype(t), decltype(t_cloned)>);

  // Raw PTR
  auto t_ptr_cloned = utils::clone(&t);
  REQUIRE(t == *t_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(&t), decltype(t_ptr_cloned)>);

  // Shared PTR
  std::shared_ptr<Transform> t_shared_ptr = std::make_shared<Transform>(t);
  auto t_shared_ptr_cloned = utils::clone(t_shared_ptr);
  REQUIRE(*t_shared_ptr == *t_shared_ptr_cloned);
  REQUIRE(t_shared_ptr.get() != t_shared_ptr_cloned.get());
  REQUIRE(t_shared_ptr != t_shared_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(t_shared_ptr), decltype(t_shared_ptr_cloned)>);

  // Weak PTR
  std::weak_ptr<Transform> t_weak_ptr = t_shared_ptr;
  auto t_weak_ptr_cloned = utils::clone(t_weak_ptr);
  REQUIRE(*t_weak_ptr.lock() == *t_weak_ptr_cloned.lock());
  // REQUIRE(t_weak_ptr == t_weak_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(t_weak_ptr), decltype(t_weak_ptr_cloned)>);

  // Optional
  std::optional<Transform> t_opt = t;
  auto t_opt_cloned = utils::clone(t_opt);
  REQUIRE(t_opt == t_opt_cloned);
  REQUIRE(t_opt.value() == t_opt_cloned.value());
  REQUIRE(std::is_same_v<decltype(t_opt), decltype(t_opt_cloned)>);

  // Shared PTR nullptr
  t_shared_ptr = nullptr;
  t_shared_ptr_cloned = utils::clone(t_shared_ptr);
  REQUIRE(t_shared_ptr == t_shared_ptr_cloned);
  REQUIRE(t_shared_ptr.get() == t_shared_ptr_cloned.get());

  // Weak PTR expired
  t_weak_ptr = std::weak_ptr<Transform>();
  t_weak_ptr_cloned = utils::clone(t_weak_ptr);
  REQUIRE(t_weak_ptr.expired());
  REQUIRE(t_weak_ptr_cloned.expired());

  // Optional nullopt
  t_opt = std::nullopt;
  t_opt_cloned = utils::clone(t_opt);
  REQUIRE(t_opt == t_opt_cloned);
  REQUIRE(!t_opt.has_value());
  REQUIRE(!t_opt_cloned.has_value());
  REQUIRE(std::is_same_v<decltype(t_opt), decltype(t_opt_cloned)>);
}

}  // namespace urx::utils::test