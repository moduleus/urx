#pragma once

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
  inline bool operator==(const ReceiveSetup& other) const {
    return ((probe_transform == other.probe_transform) && (probe == other.probe) &&
            is_nan_or_equal(sampling_frequency, other.sampling_frequency) &&
            (number_samples == other.number_samples) &&
            (channel_mapping == other.channel_mapping) && (tgc_profile == other.tgc_profile) &&
            is_nan_or_equal(tgc_sampling_frequency, other.tgc_sampling_frequency) &&
            is_nan_or_equal(modulation_frequency, other.modulation_frequency) &&
            is_nan_or_equal(time_offset, other.time_offset));
  }
  inline bool operator!=(const ReceiveSetup& other) const { return !(*this == other); }

  /// Probe used for this receive setup
  std::shared_ptr<Probe> probe;

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
