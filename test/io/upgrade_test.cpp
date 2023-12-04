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
#include <urx/wave.h>
#include <urx_utils/io/reader.h>
#include <urx_utils/io/upgrade.h>
#include <urx_utils/io/writer.h>

namespace urx::test {

TEST_CASE("Upgrade HDF5 file", "[hdf5_writer]") {
  const std::string dir = TEST_RESOURCE_DIR;

  const std::shared_ptr<urx::Dataset> dataset = urx::Upgrade::LoadFromFile(dir + "v_0_5_short.urx");
  urx::Writer::saveToFile(dir + "v_0_5_short.urx.res", *dataset);

  const std::shared_ptr<urx::Dataset> dataset2 =
      urx::Reader::loadFromFile(dir + "v_0_5_short.urx.res");
  REQUIRE(dataset2->acquisition.probes == dataset->acquisition.probes);
  REQUIRE(dataset2->acquisition.excitations == dataset->acquisition.excitations);
  REQUIRE(dataset2->acquisition.waves == dataset->acquisition.waves);
  REQUIRE(dataset2->acquisition.groups == dataset->acquisition.groups);
  REQUIRE(dataset2->acquisition.groups_data == dataset->acquisition.groups_data);
  REQUIRE(*dataset2 == *dataset);
}

}  // namespace urx::test
