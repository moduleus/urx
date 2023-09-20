#include <catch2/catch.hpp>

#include <iostream>

#include <uff_utils/group_data_reader.h>

namespace uff::test {

TEST_CASE("Read back group data complex float data", "[group_data]") {
  using DataType = std::complex<int32_t>;

  auto group = std::make_shared<uff::Group>();
  group->sampling_type = uff::Group::SamplingType::IQ;

  auto &sequence = group->sequence;

  auto receive_setup_c_3_s_2 = std::make_shared<uff::ReceiveSetup>();
  receive_setup_c_3_s_2->channel_mapping.resize(3);
  receive_setup_c_3_s_2->number_samples = 2;

  auto receive_setup_c_4_s_3 = std::make_shared<uff::ReceiveSetup>();
  receive_setup_c_4_s_3->channel_mapping.resize(4);
  receive_setup_c_4_s_3->number_samples = 3;

  const int n_events = 5;
  sequence.timed_events.resize(n_events);
  sequence.timed_events[0].receive_setup = receive_setup_c_3_s_2;
  sequence.timed_events[1].receive_setup = receive_setup_c_4_s_3;
  sequence.timed_events[2].receive_setup = receive_setup_c_4_s_3;
  sequence.timed_events[3].receive_setup = receive_setup_c_3_s_2;
  sequence.timed_events[4].receive_setup = receive_setup_c_3_s_2;

  uff::GroupData group_data;
  group_data.data_type = uff::GroupData::DataType::INT32;
  group_data.group = group;
  const int n_frames = 3;
  const int size = n_frames * 42;  // (42 = 3*2*3+4*3*2)
  group_data.raw_data.resize(size * sizeof(DataType));
  auto raw_data_ptr = reinterpret_cast<DataType *>(group_data.raw_data.data());
  for (std::size_t i = 0; i < size; i++) {
    raw_data_ptr[i] = DataType{static_cast<int32_t>(i * 2), static_cast<int32_t>(i * 2 + 1)};
  }

  uff::GroupDataReader group_data_reader{group_data};

  REQUIRE(group_data_reader.size() == size);
  REQUIRE(group_data_reader.framesCount() == n_frames);
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

  std::size_t i = 0;
  for (std::size_t fi = 0; fi < group_data_reader.framesCount(); fi++) {
    std::cout << "f" << fi << " {" << std::endl;
    for (std::size_t ei = 0; ei < group_data_reader.eventsCount(); ei++) {
      std::cout << "  e" << ei << " {" << std::endl;
      for (std::size_t ci = 0; ci < group_data_reader.channelsCount(ei); ci++) {
        std::cout << "    c" << ei << " [ ";
        for (std::size_t si = 0; si < group_data_reader.samplesCount(ei); si++) {
          std::cout << group_data_reader.at<DataType>(fi, ei, ci, si) << " ";
          REQUIRE(group_data_reader.at<DataType>(fi, ei, ci, si) == raw_data_ptr[i++]);
        }
        std::cout << "]" << std::endl;
      }
      std::cout << "  }" << std::endl;
    }
    std::cout << "}" << std::endl;
  }
}

}  // namespace uff::test
