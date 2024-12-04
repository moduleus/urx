#include <memory>

#include <catch2/catch_test_macros.hpp>
#include <test/utils/clone_generic_test.h>

namespace urx::utils::test {

TEST_CASE("Clone Vector2D", "[Clone]") {
  Vector2D<double> d_vec = {1.23, 4.56};
  generic_clone_test(d_vec);
}

}  // namespace urx::utils::test