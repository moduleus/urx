#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/probe.h>
#include <urx/v0_3/sampling_type.h>

namespace urx::v0_3 {

struct ReceiveSetup {
  double time_offset;

  std::weak_ptr<Probe> probe;
  double sampling_frequency;
  uint32_t nb_samples;
  SamplingType sampling_type;
  std::vector<std::vector<uint32_t>> channel_mapping;
  std::vector<double> tgc_profile;
  double tgc_sampling_frequency;
  double modulation_frequency;
};

}  // namespace urx::v0_3
