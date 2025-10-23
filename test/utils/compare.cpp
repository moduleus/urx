#include <memory>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/vector.h>

namespace urx::utils::test {

TEST_CASE("Compare 2 smart pointer of Datasets by value", "[Compare]") {
  const std::shared_ptr<Dataset> sd = std::make_shared<Dataset>();
  const std::shared_ptr<Dataset> sd_2 = std::make_shared<Dataset>();
  const std::weak_ptr<Dataset> wd = sd;

  REQUIRE(valueComparison(sd, sd_2));
  REQUIRE(*sd == *sd_2);
  REQUIRE(sd != sd_2);
  REQUIRE(sd.get() != sd_2.get());

  REQUIRE(valueComparison(sd, wd));
  REQUIRE(valueComparison(sd_2, wd));
  REQUIRE(valueComparison(wd, sd));
  REQUIRE(valueComparison(wd, sd_2));
}

TEST_CASE("Compare 2 vectors of smart pointer of Vector2D by value", "[Compare]") {
  const std::shared_ptr<Vector2D<int>> sv = std::make_shared<Vector2D<int>>(Vector2D<int>{1, 2});
  const std::shared_ptr<Vector2D<int>> sv_2 = std::make_shared<Vector2D<int>>(Vector2D<int>{3, 4});

  const std::shared_ptr<Vector2D<int>> sv_3 = std::make_shared<Vector2D<int>>(Vector2D<int>{1, 2});
  const std::shared_ptr<Vector2D<int>> sv_4 = std::make_shared<Vector2D<int>>(Vector2D<int>{3, 4});

  const std::weak_ptr<Vector2D<int>> wd = sv;
  const std::weak_ptr<Vector2D<int>> wd_2 = sv_2;

  const std::vector<std::shared_ptr<Vector2D<int>>> sh_vec = {sv, sv_2};
  const std::vector<std::shared_ptr<Vector2D<int>>> sh_vec_2 = {sv_3, sv_4};

  const std::vector<std::shared_ptr<Vector2D<int>>> w_vec = {sv, sv_2};
  const std::vector<std::shared_ptr<Vector2D<int>>> w_vec_2 = {sv_3, sv_4};

  REQUIRE(valueComparison(sh_vec, sh_vec_2));
  REQUIRE(sh_vec != sh_vec_2);

  REQUIRE(valueComparison(w_vec, w_vec_2));
  REQUIRE(w_vec != w_vec_2);
}

}  // namespace urx::utils::test