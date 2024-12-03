#include <memory>

#include <catch2/catch_test_macros.hpp>

#include <urx/utils/clone.h>

namespace urx::utils::test {

TEST_CASE("Clone Vector2D", "[Clone]") {
  Vector2D<double> d_vec = {1.23, 4.56};
  auto d_vec_cloned = utils::clone(d_vec);
  REQUIRE(d_vec == d_vec_cloned);
  REQUIRE(std::is_same_v<decltype(d_vec), decltype(d_vec_cloned)>);

  // Raw PTR
  auto d_vec_ptr_cloned = utils::clone(&d_vec);
  REQUIRE(d_vec == *d_vec_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(&d_vec), decltype(d_vec_ptr_cloned)>);

  // Shared PTR
  std::shared_ptr<Vector2D<double>> d_vec_shared_ptr = std::make_shared<Vector2D<double>>(d_vec);
  auto d_vec_shared_ptr_cloned = utils::clone(d_vec_shared_ptr);
  REQUIRE(*d_vec_shared_ptr == *d_vec_shared_ptr_cloned);
  REQUIRE(d_vec_shared_ptr.get() != d_vec_shared_ptr_cloned.get());
  REQUIRE(d_vec_shared_ptr != d_vec_shared_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(d_vec_shared_ptr), decltype(d_vec_shared_ptr_cloned)>);

  // Weak PTR
  std::weak_ptr<Vector2D<double>> d_vec_weak_ptr = d_vec_shared_ptr;
  auto d_vec_weak_ptr_cloned = utils::clone(d_vec_weak_ptr);
  REQUIRE(*d_vec_weak_ptr.lock() == *d_vec_weak_ptr_cloned.lock());
  // REQUIRE(d_vec_weak_ptr == d_vec_weak_ptr_cloned);
  REQUIRE(std::is_same_v<decltype(d_vec_weak_ptr), decltype(d_vec_weak_ptr_cloned)>);

  // Optional
  std::optional<Vector2D<double>> d_vec_opt = d_vec;
  auto d_vec_opt_cloned = utils::clone(d_vec_opt);
  REQUIRE(d_vec_opt == d_vec_opt_cloned);
  REQUIRE(d_vec_opt.value() == d_vec_opt_cloned.value());
  REQUIRE(std::is_same_v<decltype(d_vec_opt), decltype(d_vec_opt_cloned)>);

  // Shared PTR nullptr
  d_vec_shared_ptr = nullptr;
  d_vec_shared_ptr_cloned = utils::clone(d_vec_shared_ptr);
  REQUIRE(d_vec_shared_ptr == d_vec_shared_ptr_cloned);
  REQUIRE(d_vec_shared_ptr.get() == d_vec_shared_ptr_cloned.get());

  // Weak PTR expired
  d_vec_weak_ptr = std::weak_ptr<Vector2D<double>>();
  d_vec_weak_ptr_cloned = utils::clone(d_vec_weak_ptr);
  REQUIRE(d_vec_weak_ptr.expired());
  REQUIRE(d_vec_weak_ptr_cloned.expired());

  // Optional nullopt
  d_vec_opt = std::nullopt;
  d_vec_opt_cloned = utils::clone(d_vec_opt);
  REQUIRE(d_vec_opt == d_vec_opt_cloned);
  REQUIRE(!d_vec_opt.has_value());
  REQUIRE(!d_vec_opt_cloned.has_value());
  REQUIRE(std::is_same_v<decltype(d_vec_opt), decltype(d_vec_opt_cloned)>);
}

}  // namespace urx::utils::test