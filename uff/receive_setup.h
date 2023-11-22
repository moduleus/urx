#pragma once

#include <uff/detail/compare.h>
#include <uff/probe.h>
#include <uff/uff.h>

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace uff {

/**
 * @brief The UFF class describing the reception of the wave echos
 */
struct ReceiveSetup {
  bool operator==(const ReceiveSetup& other) const = default;

  /// Probe used for this receive setup
  std::weak_ptr<Probe> probe = std::weak_ptr<Probe>();

  /// Location of the probe in space reference for this ReceiveSetup
  Transform probe_transform;

  /// Sampling frequency [Hz]
  double sampling_frequency = UFF_NAN;

  /// Number of samples
  uint32_t number_samples = 0;

  /// Maps probe elements to receive channels.
  /// The first dimension is the probe element index, the second is an array of receive channel indexes.
  /// The receive channel index is the index of the data in group_data.data.
  /// When the channel_mapping is not defined it is assumed that the n-th receive channel maps to the n-th probe element.
  /// If probe element i-th is not connected then channel_mapping[i] is empty.
  std::vector<std::vector<uint32_t>> channel_mapping;

  /// (Optional) Analog TGC profile sampled at tgc_sampling_frequency [dB]
  std::vector<double> tgc_profile;

  /// (Optional) Sampling frequency of the TGC profile [Hz]
  double tgc_sampling_frequency = UFF_NAN;

  /// (Optional) Modulation frequency used in case of IQ-data [Hz]
  double modulation_frequency = UFF_NAN;

  /// Time offset delaying the launch of the reception related to the beginning of the event
  double time_offset = UFF_NAN;
};

}  // namespace uff
