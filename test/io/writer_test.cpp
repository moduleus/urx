#include <algorithm>
#include <chrono>
#include <complex>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/benchmark/catch_chronometer.hpp>
#include <catch2/benchmark/catch_estimate.hpp>
#include <catch2/benchmark/detail/catch_benchmark_stats.hpp>
#include <catch2/catch_config.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/interfaces/catch_interfaces_config.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include "../resources.h"

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/probe.h>
#include <urx/utils/exception.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/reader_options.h>
#include <urx/utils/io/stream.h>
#include <urx/utils/io/test/io.h>
#include <urx/utils/io/writer.h>
#include <urx/utils/io/writer_options.h>
#include <urx/utils/raw_data_helper.h>

namespace {
size_t byte_count = 0;
}

class ComputeBandwidthReporter : public Catch::EventListenerBase {
 public:
  using Catch::EventListenerBase::EventListenerBase;

  void benchmarkEnded(Catch::BenchmarkStats<> const& benchmark_stats) override {
    std::cout << "\n[speed] "
              << byte_count / (benchmark_stats.mean.point.count() / 1'000'000'000) / 1024 / 1024
              << "Mo/s\n";
  }
};

class ConfigDataSession : public Catch::Session {
 public:
  ConfigDataSession() {
    auto& config_data = configData();
    config_data.benchmarkSamples = 10;
  }
};

CATCH_REGISTER_LISTENER(ComputeBandwidthReporter)

int main(int argc, char* argv[]) {
  ConfigDataSession session;
  return session.run(argc, argv);
}

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
      std::make_shared<RawDataVector<std::complex<double>>>(
          std::vector<std::complex<double>>{{1.2, 9.5}, {3.4, -6.4}, {-5.6, -99.8}, {5.6, 99.8}});
  const std::shared_ptr<urx::RawData> raw_data_vector_double2 =
      std::make_shared<RawDataVector<std::complex<double>>>(
          std::vector<std::complex<double>>{{12., -11.2}, {34., -7.1}, {-56., 8.7}, {15.3, -1.2}});
  const std::shared_ptr<urx::RawData> raw_data_vector_short =
      std::make_shared<RawDataVector<short>>(std::vector<short>{12, 456, -12, -456});
  const std::shared_ptr<urx::RawData> raw_data_vector_short2 =
      std::make_shared<RawDataVector<short>>(std::vector<short>{987, 654, 312, -312});

  const std::shared_ptr<urx::RawDataVector<std::complex<double>>> raw_data_double =
      std::make_shared<urx::RawDataVector<std::complex<double>>>(
          raw_data_vector_double->getSize() + raw_data_vector_double2->getSize());
  memcpy(static_cast<std::complex<double>*>(raw_data_double->getBuffer()),
         raw_data_vector_double->getBuffer(),
         sizeof(std::complex<double>) * raw_data_vector_double->getSize());
  memcpy(static_cast<std::complex<double>*>(raw_data_double->getBuffer()) +
             raw_data_vector_double->getSize(),
         raw_data_vector_double2->getBuffer(),
         sizeof(std::complex<double>) * raw_data_vector_double2->getSize());

  const std::shared_ptr<urx::RawDataVector<short>> raw_data_short =
      std::make_shared<urx::RawDataVector<short>>(raw_data_vector_short->getSize() +
                                                  raw_data_vector_short2->getSize());
  memcpy(static_cast<short*>(raw_data_short->getBuffer()), raw_data_vector_short->getBuffer(),
         sizeof(short) * raw_data_vector_short->getSize());
  memcpy(static_cast<short*>(raw_data_short->getBuffer()) + raw_data_vector_short->getSize(),
         raw_data_vector_short2->getBuffer(), sizeof(short) * raw_data_vector_short2->getSize());

  // Write using stream.
  {
    Stream stream(filename, dataset);
    stream.saveToFile();

    {
      stream.setChunkGroupData(false);
      urx::utils::io::GroupDataStream group_data =
          stream.createGroupData(dataset->acquisition.groups.front(), urx::DoubleNan(1.));

      group_data.append(raw_data_vector_double, 1.2, {2.3, 3.4, 4.5});
      group_data.append(raw_data_vector_double2, 12., {23., 34., 45.});
    }

    {
      stream.setChunkGroupData(true);
      urx::utils::io::GroupDataStream group_data =
          stream.createGroupData(dataset->acquisition.groups.front(), urx::DoubleNan(2.));

      group_data.append(raw_data_vector_short, 9., {8.});
      group_data.append(raw_data_vector_short2, 5., {8., 9.4});
    }
  }

  // Full read
  {
    auto dataset_loaded = reader::loadFromFile(filename);

    {
      const urx::GroupData& group_data_double =
          dataset_loaded->acquisition
              .groups_data[dataset_loaded->acquisition.groups_data.size() - 2];

      REQUIRE(group_data_double.sequence_timestamps == std::vector<double>{1.2, 12});
      REQUIRE(group_data_double.event_timestamps ==
              std::vector<std::vector<double>>{{2.3, 3.4, 4.5}, {23., 34., 45.}});
      REQUIRE(*group_data_double.raw_data == *raw_data_double);
    }

    {
      const urx::GroupData& group_data_short =
          dataset_loaded->acquisition
              .groups_data[dataset_loaded->acquisition.groups_data.size() - 1];

      REQUIRE(group_data_short.sequence_timestamps == std::vector<double>{9., 5.});
      REQUIRE(group_data_short.event_timestamps ==
              std::vector<std::vector<double>>{{8.}, {8., 9.4}});
      REQUIRE(*group_data_short.raw_data == *raw_data_short);
    }
  }

  // Stream read
  {
    const std::shared_ptr<Dataset> dataset_loaded = std::make_shared<Dataset>();
    Stream stream(filename, dataset_loaded);
    stream.setRawDataLoadPolicy(urx::utils::io::RawDataLoadPolicy::STREAM);
    stream.loadFromFile();

    const std::shared_ptr<urx::RawData> buffer_double =
        std::make_shared<urx::RawDataVector<std::complex<double>>>(8);
    const std::shared_ptr<urx::RawData> buffer_short =
        std::make_shared<urx::RawDataVector<short>>(8);
    {
      stream.readRawData(dataset_loaded->acquisition.groups_data.size() - 2, buffer_double, 0, 0,
                         1);
      stream.readRawData(dataset_loaded->acquisition.groups_data.size() - 2, buffer_double, 1, 1,
                         1);

      REQUIRE(*buffer_double == *raw_data_double);
    }

    {
      stream.readRawData(dataset_loaded->acquisition.groups_data.size() - 1, buffer_short, 0, 0, 1);
      stream.readRawData(dataset_loaded->acquisition.groups_data.size() - 1, buffer_short, 1, 1, 1);

      REQUIRE(*buffer_short == *raw_data_short);
    }
  }
}

TEST_CASE("Benchmark stream HDF5 file", "[hdf5_reader][hdf5_writer]") {
  std::string filename = ::test::getDataTestPath() + "/rca.urx";

  auto i = GENERATE(1, 10, 100, 1000);

  BENCHMARK_ADVANCED("Stream write " + std::to_string(i) + " seq")
  (Catch::Benchmark::Chronometer meter) {
    if (std::filesystem::exists(filename + ".tmp")) {
      REQUIRE(std::filesystem::remove(filename + ".tmp"));
    }
    REQUIRE(std::filesystem::copy_file(filename, filename + ".tmp"));

    const std::shared_ptr<Dataset> dataset_loaded = std::make_shared<Dataset>();
    Stream stream(filename + ".tmp", dataset_loaded);
    stream.setRawDataLoadPolicy(urx::utils::io::RawDataLoadPolicy::STREAM);
    stream.setChunkGroupData(true);
    stream.loadFromFile();

    urx::utils::io::GroupDataStream group_data =
        stream.createGroupData(dataset_loaded->acquisition.groups.front(), urx::DoubleNan(1.));

    const urx::utils::GroupDataReader group_data_reader(group_data.getGroupData());

    const std::shared_ptr<urx::RawDataVector<short>> raw_data =
        std::make_shared<urx::RawDataVector<short>>(group_data_reader.offset(1, 0, 0, 0));

    const urx::utils::RawDataHelper raw_data_helper(raw_data.get());

    byte_count = raw_data->getSize() * raw_data_helper.sizeofSample() * i;

    std::vector<double> event_timestamp(group_data.getGroupData().group.lock()->sequence.size());

    meter.measure(
        [&group_data2 = group_data, &raw_data2 = raw_data, &event_timestamp2 = event_timestamp, i] {
          for (int j = 0; j < i; j++) {
            group_data2.append(raw_data2, 1.2, event_timestamp2);
          }
        });
  };

  BENCHMARK_ADVANCED("Stream read " + std::to_string(i) + " seq")
  (Catch::Benchmark::Chronometer meter) {
    const std::shared_ptr<Dataset> dataset_loaded = std::make_shared<Dataset>();
    Stream stream(filename + ".tmp", dataset_loaded);
    stream.setRawDataLoadPolicy(urx::utils::io::RawDataLoadPolicy::STREAM);
    stream.loadFromFile();

    const urx::utils::GroupDataReader group_data_reader(dataset_loaded->acquisition.groups_data[0]);

    const std::shared_ptr<urx::RawDataVector<short>> raw_data =
        std::make_shared<urx::RawDataVector<short>>(group_data_reader.offset(1, 0, 0, 0));

    const urx::utils::RawDataHelper raw_data_helper(raw_data.get());

    byte_count = raw_data->getSize() * raw_data_helper.sizeofSample() * i;

    meter.measure([&stream2 = stream, &raw_data2 = raw_data, i] {
      for (int j = 0; j < i; j++) {
        stream2.readRawData(0, raw_data2, 0, j, 1);
      }
    });
  };
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
