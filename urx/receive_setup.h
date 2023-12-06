#pragma once

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/probe.h>
#include <urx/urx.h>

namespace urx {

/**
 * @brief The URX class describing the reception of the wave echos
 */
struct ReceiveSetup {
  bool operator==(const ReceiveSetup& other) const = default;

  std::weak_ptr<Probe> probe = std::weak_ptr<Probe>();

  Transform probe_transform;

  DoubleNan sampling_frequency;

  uint32_t number_samples = 0;

  std::vector<std::vector<uint32_t>> channel_mapping;

  std::vector<double> tgc_profile;
  DoubleNan tgc_sampling_frequency;

  DoubleNan modulation_frequency;
  DoubleNan time_offset{0};
};

}  // namespace urx
