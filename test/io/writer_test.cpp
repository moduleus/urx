#include <algorithm>
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
#include <urx/utils/exception.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/test/io.h>
#include <urx/utils/io/writer.h>

namespace urx::utils::io::test {

TEST_CASE("Write HDF5 file", "[hdf5_writer][hdf5_reader]") {
  auto dataset = generateFakeDataset<Dataset>();

  writer::saveToFile("writer康🐜.urx", *dataset);

  auto dataset_loaded = reader::loadFromFile("writer康🐜.urx");

  REQUIRE(dataset_loaded->acquisition.probes == dataset->acquisition.probes);
  REQUIRE(dataset_loaded->acquisition.excitations == dataset->acquisition.excitations);
  REQUIRE(dataset_loaded->acquisition.groups == dataset->acquisition.groups);
  REQUIRE(dataset_loaded->acquisition.groups_data == dataset->acquisition.groups_data);
  REQUIRE(*dataset_loaded == *dataset);
}

TEST_CASE("Read failure HDF5 file", "[hdf5_reader]") {
  REQUIRE_THROWS_AS(reader::loadFromFile("missing_file.urx"), urx::utils::ReadFileException);
}

TEST_CASE("Write failure HDF5 file", "[hdf5_writer]") {
  const urx::Dataset dataset;
#ifdef _WIN32
  REQUIRE_THROWS_AS(writer::saveToFile("aux", dataset), urx::utils::WriteFileException);
#else
  REQUIRE_THROWS_AS(writer::saveToFile("/", dataset), urx::utils::WriteFileException);
#endif
}

}  // namespace urx::utils::io::test
