#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/probe.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/upgrade.h>
#include <urx/utils/io/writer.h>

namespace urx::utils::io::test {

TEST_CASE("Upgrade HDF5 v0.2 file", "[hdf5_writer]") {
  const std::string dir = TEST_RESOURCE_DIR;

  const std::shared_ptr<Dataset> dataset = Upgrade::LoadFromFile(dir + "v_0_2_short.urx");
  Writer::saveToFile(dir + "v_0_2_short.urx.res", *dataset);

  const std::shared_ptr<Dataset> dataset2 = Reader::loadFromFile(dir + "v_0_2_short.urx.res");
  REQUIRE(dataset2->acquisition.probes == dataset->acquisition.probes);
  REQUIRE(dataset2->acquisition.excitations == dataset->acquisition.excitations);
  REQUIRE(dataset2->acquisition.groups == dataset->acquisition.groups);
  REQUIRE(dataset2->acquisition.groups_data == dataset->acquisition.groups_data);
  REQUIRE(*dataset2 == *dataset);
  REQUIRE(std::filesystem::remove(dir + "v_0_2_short.urx.res"));
}

TEST_CASE("Upgrade HDF5 v0.3 file", "[hdf5_writer]") {
  const std::string dir = TEST_RESOURCE_DIR;

  std::shared_ptr<urx::Dataset> dataset =
      urx::utils::io::Upgrade::LoadFromFile(dir + "v_0_3_rcaarray.uff");
  urx::utils::io::Writer::saveToFile(dir + "v_0_3_rcaarray.urx.res", *dataset);
  std::shared_ptr<urx::Dataset> dataset2 =
      urx::utils::io::Reader::loadFromFile(dir + "v_0_3_rcaarray.urx.res");
  REQUIRE(*dataset2 == *dataset);
  dataset.reset();
  dataset2.reset();

  dataset = urx::utils::io::Upgrade::LoadFromFile(dir + "v_0_3_channel_mapping.uff");
  urx::utils::io::Writer::saveToFile(dir + "v_0_3_channel_mapping.urx.res", *dataset);
  dataset2 = urx::utils::io::Reader::loadFromFile(dir + "v_0_3_channel_mapping.urx.res");
  REQUIRE(*dataset2 == *dataset);
  dataset.reset();
  dataset2.reset();

  dataset = urx::utils::io::Upgrade::LoadFromFile(dir + "v_0_3_group_data.uff");
  urx::utils::io::Writer::saveToFile(dir + "v_0_3_group_data.urx.res", *dataset);
  dataset2 = urx::utils::io::Reader::loadFromFile(dir + "v_0_3_group_data.urx.res");
  REQUIRE(*dataset2 == *dataset);
  dataset.reset();
  dataset2.reset();

  dataset = urx::utils::io::Upgrade::LoadFromFile(dir + "v_0_3_group_links.uff");
  urx::utils::io::Writer::saveToFile(dir + "v_0_3_group_links.urx.res", *dataset);
  dataset2 = urx::utils::io::Reader::loadFromFile(dir + "v_0_3_group_links.urx.res");
  REQUIRE(*dataset2 == *dataset);
  dataset.reset();
  dataset2.reset();
}

}  // namespace urx::utils::io::test
