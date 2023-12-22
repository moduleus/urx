#include <complex>
#include <cstdint>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <urx/detail/raw_data.h>
#include <urx/event.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/receive_setup.h>
#include <urx_utils/group_data_reader.h>

namespace urx::test {

TEST_CASE("Read back group data complex float data", "[group_data]") {
  using CompInt32 = std::complex<int32_t>;

  auto group = std::make_shared<urx::Group>();
  group->sampling_type = urx::Group::SamplingType::IQ;
  group->data_type = urx::Group::DataType::INT32;

  auto& sequence = group->sequence;

  auto receive_setup_c_3_s_2 = urx::ReceiveSetup();
  receive_setup_c_3_s_2.channel_mapping = {{1}, {0, 1}, {2}, {0}};
  receive_setup_c_3_s_2.number_samples = 2;

  auto receive_setup_c_4_s_3 = urx::ReceiveSetup();
  receive_setup_c_4_s_3.channel_mapping = {{3}, {2, 1}, {0}, {2}};
  receive_setup_c_4_s_3.number_samples = 3;

  const int n_events = 5;
  sequence.resize(n_events);
  sequence[0].receive_setup = receive_setup_c_3_s_2;
  sequence[1].receive_setup = receive_setup_c_4_s_3;
  sequence[2].receive_setup = receive_setup_c_4_s_3;
  sequence[3].receive_setup = receive_setup_c_3_s_2;
  sequence[4].receive_setup = receive_setup_c_3_s_2;

  urx::GroupData group_data;
  group_data.group = group;
  const int n_sequences = 3;
  const int size = n_sequences * 42;  // (42 = 3*2*3+4*3*2)

  group_data.raw_data = std::make_shared<RawDataNoInit<CompInt32>>(size);

  CompInt32* raw_data_ptr = static_cast<CompInt32*>(group_data.raw_data->getBuffer());
  for (int i = 0; i < size; ++i) {
    raw_data_ptr[i] = {i * 2, i * 2 + 1};
  }

  urx::GroupDataReader group_data_reader{group_data};

  REQUIRE(group_data_reader.sequencesCount() == n_sequences);
  REQUIRE(group_data_reader.eventsCount() == n_events);
  REQUIRE(group_data_reader.channelsCount(0) == 3);
  REQUIRE(group_data_reader.channelsCount(1) == 4);
  REQUIRE(group_data_reader.channelsCount(2) == 4);
  REQUIRE(group_data_reader.channelsCount(3) == 3);
  REQUIRE(group_data_reader.channelsCount(4) == 3);
  REQUIRE(group_data_reader.samplesCount(0) == 2);
  REQUIRE(group_data_reader.samplesCount(1) == 3);
  REQUIRE(group_data_reader.samplesCount(2) == 3);
  REQUIRE(group_data_reader.samplesCount(3) == 2);
  REQUIRE(group_data_reader.samplesCount(4) == 2);

  size_t i = 0;
  for (size_t fi = 0; fi < group_data_reader.sequencesCount(); fi++) {
    // std::cout << "f" << fi << " {\n";
    for (size_t ei = 0; ei < group_data_reader.eventsCount(); ei++) {
      // std::cout << "  e" << ei << " {\n";
      for (size_t ci = 0; ci < group_data_reader.channelsCount(ei); ci++) {
        // std::cout << "    c" << ei << " [ ";
        for (size_t si = 0; si < group_data_reader.samplesCount(ei); si++) {
          // std::cout << group_data_reader.at<CompInt32>(fi, ei, ci, si) << " ";
          REQUIRE(group_data_reader.at<CompInt32>(fi, ei, ci, si) == raw_data_ptr[i++]);
        }
        // std::cout << "]\n";
      }
      // std::cout << "  }\n";
    }
    // std::cout << "}\n";
  }
}

}  // namespace urx::test
