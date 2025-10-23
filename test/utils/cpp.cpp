#include <memory>
#include <optional>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/utils/cpp.h>

namespace urx::utils::test {

TEST_CASE("IsSharedPtr", "[Cpp]") {
  REQUIRE(!urx::utils::IsSharedPtr<double>::value);
  REQUIRE(!urx::utils::IsSharedPtr<double*>::value);
  REQUIRE(urx::utils::IsSharedPtr<std::shared_ptr<double>>::value);
  REQUIRE(!urx::utils::IsSharedPtr<std::weak_ptr<double>>::value);
  REQUIRE(!urx::utils::IsSharedPtr<std::optional<double>>::value);
}

TEST_CASE("IsWeakPtr", "[Cpp]") {
  REQUIRE(!urx::utils::IsWeakPtr<double>::value);
  REQUIRE(!urx::utils::IsWeakPtr<double*>::value);
  REQUIRE(!urx::utils::IsWeakPtr<std::shared_ptr<double>>::value);
  REQUIRE(urx::utils::IsWeakPtr<std::weak_ptr<double>>::value);
  REQUIRE(!urx::utils::IsWeakPtr<std::optional<double>>::value);
}

TEST_CASE("IsOptional", "[Cpp]") {
  REQUIRE(!urx::utils::IsOptional<double>::value);
  REQUIRE(!urx::utils::IsOptional<double*>::value);
  REQUIRE(!urx::utils::IsOptional<std::shared_ptr<double>>::value);
  REQUIRE(!urx::utils::IsOptional<std::weak_ptr<double>>::value);
  REQUIRE(urx::utils::IsOptional<std::optional<double>>::value);
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