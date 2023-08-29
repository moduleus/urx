#pragma once

#include <uff/probe.h>
#include <uff/time_offset_base.h>
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
class ReceiveSetup {
 public:
  enum class SAMPLING_TYPE { DIRECT_RF = 0, IQ = 1 };
  
  ReceiveSetup(std::weak_ptr<Probe> probe, double sampling_frequency, uint32_t number_samples,
               double time_offset = 0.)
      : _probe(std::move(probe)),
        _sampling_frequency(sampling_frequency),
        _number_samples(number_samples),
        _time_offset(time_offset) {}
        
  inline bool operator==(const ReceiveSetup& other) const {
    return ((_probe.expired() == other._probe.expired()) &&
            (_probe.expired() || (*(_probe.lock()) == *(other._probe.lock()))) &&
            (_sampling_frequency == other._sampling_frequency) &&
            (_number_samples == other._number_samples) &&
            (_sampling_type == other._sampling_type) &&
            (_channel_mapping == other._channel_mapping) && (_tgc_profile == other._tgc_profile) &&
            (_tgc_sampling_frequency == other._tgc_sampling_frequency) &&
            (_modulation_frequency == other._modulation_frequency) &&
            (_time_offset == other._time_offset));
  }
  inline bool operator!=(const ReceiveSetup& other) const { return !(*this == other); }

  // Probes used for this receive setup
  std::weak_ptr<Probe> _probe;

  // Sampling frequency [Hz]
  double _sampling_frequency = 0.;

  // Number of samples
  uint32_t _number_samples = 0;

  // Type of sampling:
  SAMPLING_TYPE _sampling_type = SAMPLING_TYPE::DIRECT_RF;

  // Maps the RF line with the corresponding element
  // channel_element_index = _channel_mapping[rf_index]
  std::vector<int> _channel_mapping;

  // (Optional) Analog TGC profile sampled at tgc_sampling_frequency [dB]
  std::vector<double> _tgc_profile = {0.f};

  // (Optional) Sampling frequency of the TGC profile [Hz]
  std::optional<double> _tgc_sampling_frequency = std::nullopt;

  // (Optional) Modulation frequency used in case of IQ-data [Hz]
  std::optional<double> _modulation_frequency = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
