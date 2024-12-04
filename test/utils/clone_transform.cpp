#include <memory>

#include <catch2/catch_test_macros.hpp>
#include <test/utils/clone_generic_test.h>

namespace urx::utils::test {

TEST_CASE("Clone Transform", "[Clone]") {
  Transform t = {{1, 2, 3}, {4, 5, 6}};
  generic_clone_test(t);
}

}  // namespace urx::utils::test