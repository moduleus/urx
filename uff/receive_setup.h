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
    return ((probe.expired() == other.probe.expired()) &&
            (probe.expired() || (*(probe.lock()) == *(other.probe.lock()))) &&
            (sampling_frequency == other.sampling_frequency) &&
            (number_samples == other.number_samples) &&
            (channel_mapping == other.channel_mapping) && (tgc_profile == other.tgc_profile) &&
            (tgc_sampling_frequency == other.tgc_sampling_frequency) &&
            (modulation_frequency == other.modulation_frequency) &&
            (time_offset == other.time_offset));
  }
  inline bool operator!=(const ReceiveSetup& other) const { return !(*this == other); }

  /// Probe used for this receive setup
  std::weak_ptr<Probe> probe;

  /// Sampling frequency [Hz]
  double sampling_frequency = 0.;

  /// Number of samples
  uint32_t number_samples = 0;

  /// Maps the RF line with the corresponding element
  /// channel_element_index = channel_mapping[rf_index]
  std::vector<int> channel_mapping;

  /// (Optional) Analog TGC profile sampled at tgc_sampling_frequency [dB]
  std::vector<double> tgc_profile;

  /// (Optional) Sampling frequency of the TGC profile [Hz]
  std::optional<double> tgc_sampling_frequency = std::nullopt;

  /// (Optional) Modulation frequency used in case of IQ-data [Hz]
  std::optional<double> modulation_frequency = std::nullopt;

  /// Time offset delaying the launch of the reception related to the beginning of the event
  double time_offset = 0.;
};

}  // namespace uff
