#include <algorithm>
#include <complex>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/test/io.h>
#include <urx/utils/io/writer.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx::utils::io::test {

TEST_CASE("Write HDF5 file", "[hdf5_writer]") {
  auto dataset = generateFakeDataset<Dataset>();

  writer::saveToFile("writer康🐜.urx", *dataset);

  auto dataset_loaded = reader::loadFromFile("writer康🐜.urx");

  REQUIRE(dataset_loaded->acquisition.probes == dataset->acquisition.probes);
  REQUIRE(dataset_loaded->acquisition.excitations == dataset->acquisition.excitations);
  REQUIRE(dataset_loaded->acquisition.groups == dataset->acquisition.groups);
  REQUIRE(dataset_loaded->acquisition.groups_data == dataset->acquisition.groups_data);
  REQUIRE(*dataset_loaded == *dataset);
}

}  // namespace urx::utils::io::test
