#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/probe.h>
#include <urx/utils/exception.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/stream.h>
#include <urx/utils/io/test/io.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_options.h>

namespace urx::utils::io::test {

TEST_CASE("Write HDF5 file", "[hdf5_writer][hdf5_reader]") {
  auto dataset = generateFakeDataset<Dataset>();

  auto chunk_group_data = GENERATE(false, true);

  urx::utils::io::WriterOptions options;

  options.setChunkGroupData(chunk_group_data);

  const std::string filename =
      "writer康🐜-" + std::string{chunk_group_data ? "chunk" : "contiguous"} + ".urx";

  writer::saveToFile(filename, *dataset, options);

  auto dataset_loaded = reader::loadFromFile(filename);

  REQUIRE(dataset_loaded->acquisition.probes == dataset->acquisition.probes);
  REQUIRE(dataset_loaded->acquisition.excitations == dataset->acquisition.excitations);
  REQUIRE(dataset_loaded->acquisition.groups == dataset->acquisition.groups);
  REQUIRE(dataset_loaded->acquisition.groups_data == dataset->acquisition.groups_data);
  REQUIRE(*dataset_loaded == *dataset);
}

TEST_CASE("Stream HDF5 file", "[hdf5_writer][hdf5_reader]") {
  auto dataset = generateFakeDataset<Dataset>();

  const std::string filename = "stream.urx";

  if (std::filesystem::exists(filename)) {
    REQUIRE(std::filesystem::remove(filename));
  }

  const std::shared_ptr<urx::RawData> raw_data_vector_double =
      std::make_shared<RawDataVector<double>>(std::vector<double>{1.2, 3.4, -5.6});
  const std::shared_ptr<urx::RawData> raw_data_vector_double2 =
      std::make_shared<RawDataVector<double>>(std::vector<double>{12., 34., -56., 15.3});
  const std::shared_ptr<urx::RawData> raw_data_vector_short =
      std::make_shared<RawDataVector<short>>(std::vector<short>{12, 456});
  const std::shared_ptr<urx::RawData> raw_data_vector_short2 =
      std::make_shared<RawDataVector<short>>(std::vector<short>{987, 654, 312});

  {
    Stream stream(filename, dataset);
    stream.saveToFile();

    {
      stream.getWriter().getOptions().setChunkGroupData(false);
      urx::utils::io::GroupDataStream group_data =
          stream.createGroupData(dataset->acquisition.groups.front(), urx::DoubleNan(1.));

      group_data.append(raw_data_vector_double, 1.2, {2.3, 3.4, 4.5});
      group_data.append(raw_data_vector_double2, 12., {23., 34., 45.});
    }

    {
      stream.getWriter().getOptions().setChunkGroupData(true);
      urx::utils::io::GroupDataStream group_data =
          stream.createGroupData(dataset->acquisition.groups.front(), urx::DoubleNan(2.));

      group_data.append(raw_data_vector_short, 9., {8.});
      group_data.append(raw_data_vector_short2, 5., {8., 9.4});
    }
  }

  auto dataset_loaded = reader::loadFromFile(filename);

  {
    const urx::GroupData& group_data_double =
        dataset_loaded->acquisition.groups_data[dataset_loaded->acquisition.groups_data.size() - 2];
    const std::shared_ptr<urx::RawDataVector<double>> raw_data_double =
        std::make_shared<urx::RawDataVector<double>>(raw_data_vector_double->getSize() +
                                                     raw_data_vector_double2->getSize());
    memcpy(static_cast<double*>(raw_data_double->getBuffer()), raw_data_vector_double->getBuffer(),
           sizeof(double) * raw_data_vector_double->getSize());
    memcpy(static_cast<double*>(raw_data_double->getBuffer()) + raw_data_vector_double->getSize(),
           raw_data_vector_double2->getBuffer(),
           sizeof(double) * raw_data_vector_double2->getSize());

    REQUIRE(group_data_double.sequence_timestamps == std::vector<double>{1.2, 12});
    REQUIRE(group_data_double.event_timestamps ==
            std::vector<std::vector<double>>{{2.3, 3.4, 4.5}, {23., 34., 45.}});
    REQUIRE(*group_data_double.raw_data == *raw_data_double);
  }

  {
    const urx::GroupData& group_data_short =
        dataset_loaded->acquisition.groups_data[dataset_loaded->acquisition.groups_data.size() - 1];
    const std::shared_ptr<urx::RawDataVector<short>> raw_data_short =
        std::make_shared<urx::RawDataVector<short>>(raw_data_vector_short->getSize() +
                                                    raw_data_vector_short2->getSize());
    memcpy(static_cast<short*>(raw_data_short->getBuffer()), raw_data_vector_short->getBuffer(),
           sizeof(short) * raw_data_vector_short->getSize());
    memcpy(static_cast<short*>(raw_data_short->getBuffer()) + raw_data_vector_short->getSize(),
           raw_data_vector_short2->getBuffer(), sizeof(short) * raw_data_vector_short2->getSize());

    REQUIRE(group_data_short.sequence_timestamps == std::vector<double>{9., 5.});
    REQUIRE(group_data_short.event_timestamps == std::vector<std::vector<double>>{{8.}, {8., 9.4}});
    REQUIRE(*group_data_short.raw_data == *raw_data_short);
  }
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
