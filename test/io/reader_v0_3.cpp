#include <memory>
#include <string>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#include <urx/utils/io/reader_v0_3.h>

namespace urx::test {

TEST_CASE("Reader HDF5 v0_3 file", "[uff_v0_3]") {
  const std::string dir = TEST_RESOURCE_DIR;

  auto dataset_loaded = urx::utils::io::v0_3::Reader::loadFromFile(dir + "v_0_3_rcaarray.uff");
  dataset_loaded.reset();
  dataset_loaded = urx::utils::io::v0_3::Reader::loadFromFile(dir + "v_0_3_channel_mapping.uff");
  dataset_loaded.reset();
  dataset_loaded = urx::utils::io::v0_3::Reader::loadFromFile(dir + "v_0_3_group_data.uff");
  dataset_loaded.reset();
  dataset_loaded = urx::utils::io::v0_3::Reader::loadFromFile(dir + "v_0_3_group_links.uff");
}

}  // namespace urx::test
