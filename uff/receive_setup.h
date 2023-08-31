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
 * @brief The UFF ReceiveSetup class
 */
struct ReceiveSetup {
  enum class SAMPLING_TYPE { DIRECT_RF = 0, IQ = 1 };

  inline bool operator==(const ReceiveSetup& other) const {
    return ((probe.expired() == other.probe.expired()) &&
            (probe.expired() || (*(probe.lock()) == *(other.probe.lock()))) &&
            (sampling_frequency == other.sampling_frequency) &&
            (number_samples == other.number_samples) && (sampling_type == other.sampling_type) &&
            (channel_mapping == other.channel_mapping) && (tgc_profile == other.tgc_profile) &&
            (tgc_sampling_frequency == other.tgc_sampling_frequency) &&
            (modulation_frequency == other.modulation_frequency) &&
            (time_offset == other.time_offset));
  }
  inline bool operator!=(const ReceiveSetup& other) const { return !(*this == other); }

  // Probes used for this receive setup
  std::weak_ptr<Probe> probe;

  // Sampling frequency [Hz]
  double sampling_frequency = 0.;

  // Number of samples
  uint32_t number_samples = 0;

  // Type of sampling:
  SAMPLING_TYPE sampling_type = SAMPLING_TYPE::DIRECT_RF;

  // Maps the RF line with the corresponding element
  // channel_element_index = _channel_mapping[rf_index]
  std::vector<int> channel_mapping;

  // (Optional) Analog TGC profile sampled at tgc_sampling_frequency [dB]
  std::vector<double> tgc_profile = {0.f};

  // (Optional) Sampling frequency of the TGC profile [Hz]
  std::optional<double> tgc_sampling_frequency = std::nullopt;

  // (Optional) Modulation frequency used in case of IQ-data [Hz]
  std::optional<double> modulation_frequency = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff
