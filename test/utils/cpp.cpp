#include <memory>
#include <optional>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/utils/cpp.h>

namespace urx::utils::test {

TEST_CASE("is_shared_ptr", "[Cpp]") {
  REQUIRE(!urx::utils::is_shared_ptr<double>::value);
  REQUIRE(!urx::utils::is_shared_ptr<double*>::value);
  REQUIRE(urx::utils::is_shared_ptr<std::shared_ptr<double>>::value);
  REQUIRE(!urx::utils::is_shared_ptr<std::weak_ptr<double>>::value);
  REQUIRE(!urx::utils::is_shared_ptr<std::optional<double>>::value);
}

TEST_CASE("is_weak_ptr", "[Cpp]") {
  REQUIRE(!urx::utils::is_weak_ptr<double>::value);
  REQUIRE(!urx::utils::is_weak_ptr<double*>::value);
  REQUIRE(!urx::utils::is_weak_ptr<std::shared_ptr<double>>::value);
  REQUIRE(urx::utils::is_weak_ptr<std::weak_ptr<double>>::value);
  REQUIRE(!urx::utils::is_weak_ptr<std::optional<double>>::value);
}

TEST_CASE("is_optional", "[Cpp]") {
  REQUIRE(!urx::utils::is_optional<double>::value);
  REQUIRE(!urx::utils::is_optional<double*>::value);
  REQUIRE(!urx::utils::is_optional<std::shared_ptr<double>>::value);
  REQUIRE(!urx::utils::is_optional<std::weak_ptr<double>>::value);
  REQUIRE(urx::utils::is_optional<std::optional<double>>::value);
}

TEST_CASE("TypeContainer", "[Cpp]") {
  REQUIRE(urx::utils::TypeContainer<double>::VALUE == urx::utils::ContainerType::RAW);
  REQUIRE(urx::utils::TypeContainer<double*>::VALUE == urx::utils::ContainerType::RAW);
  REQUIRE(urx::utils::TypeContainer<std::vector<double>>::VALUE ==
          urx::utils::ContainerType::VECTOR);
  REQUIRE(urx::utils::TypeContainer<std::optional<double>>::VALUE ==
          urx::utils::ContainerType::OPTIONAL);
  REQUIRE(urx::utils::TypeContainer<std::shared_ptr<double>>::VALUE ==
          urx::utils::ContainerType::SHARED_PTR);
  REQUIRE(urx::utils::TypeContainer<std::weak_ptr<double>>::VALUE ==
          urx::utils::ContainerType::WEAK_PTR);
}

}  // namespace urx::utils::test