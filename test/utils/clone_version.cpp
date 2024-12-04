#include <memory>

#include <catch2/catch_test_macros.hpp>
#include <test/utils/clone_generic_test.h>

namespace urx::utils::test {

TEST_CASE("Clone Version", "[Clone]") {
  Version vers = {1, 2, 3};
  generic_clone_test(vers);
}

}  // namespace urx::utils::test