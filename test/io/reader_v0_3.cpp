#include <catch2/catch_test_macros.hpp>

#include <urx/v0_3/dataset.h>

namespace urx::test {

TEST_CASE("Reader HDF5 v0_3 file", "[uff_v0_3]") { const urx::v0_3::Dataset dataset{}; }

}  // namespace urx::test
