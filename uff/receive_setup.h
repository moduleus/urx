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
class ReceiveSetup : public TimeOffsetBase {
 public:
  enum class SAMPLING_TYPE { DIRECT_RF = 0, IQ = 1 };

  // CTOR & DTOR
  ReceiveSetup() = delete;
  ReceiveSetup(std::weak_ptr<Probe> probe, MetadataType sampling_frequency, uint32_t number_samples,
               MetadataType time_offset = 0.)
      : TimeOffsetBase(time_offset),
        _probe(std::move(probe)),
        _sampling_frequency(sampling_frequency),
        _number_samples(number_samples) {}
  ReceiveSetup(const ReceiveSetup&) = default;
  ReceiveSetup(ReceiveSetup&&) = default;
  ~ReceiveSetup() override = default;

  // Operators
  ReceiveSetup& operator=(const ReceiveSetup& other) noexcept = default;
  ReceiveSetup& operator=(ReceiveSetup&& other) noexcept = default;
  inline bool operator==(const ReceiveSetup& other) const {
    return (TimeOffsetBase::operator==(other) && (_probe.expired() == other._probe.expired()) &&
            (_probe.expired() || (*(_probe.lock()) == *(other._probe.lock()))) &&
            (_sampling_frequency == other._sampling_frequency) &&
            (_number_samples == other._number_samples) &&
            (_sampling_type == other._sampling_type) &&
            (_channel_mapping == other._channel_mapping) && (_tgc_profile == other._tgc_profile) &&
            (_tgc_sampling_frequency == other._tgc_sampling_frequency) &&
            (_modulation_frequency == other._modulation_frequency));
  }
  inline bool operator!=(const ReceiveSetup& other) const { return !(*this == other); }

  // Accessors
  inline std::weak_ptr<Probe> probe() const { return _probe; }
  inline void setProbe(const std::weak_ptr<Probe>& probe) { _probe = probe; }

  inline MetadataType samplingFrequency() const { return _sampling_frequency; }
  inline void setSamplingFrequency(const MetadataType& samplingFrequency) {
    _sampling_frequency = samplingFrequency;
  }

  inline uint32_t numberSamples() const { return _number_samples; }
  inline void setNumberSamples(uint32_t number_samples) { _number_samples = number_samples; }

  inline SAMPLING_TYPE samplingType() const { return _sampling_type; }
  inline void setSamplingType(const SAMPLING_TYPE& samplingType) { _sampling_type = samplingType; }

  inline std::vector<int> channelMapping() const { return _channel_mapping; }
  inline void setChannelMapping(const std::vector<int>& channelMapping) {
    _channel_mapping = channelMapping;
  }

  inline std::vector<MetadataType> tgcProfile() const { return _tgc_profile; }
  inline void setTgcProfile(const std::vector<MetadataType>& tgcProfile) {
    _tgc_profile = tgcProfile;
  }

  inline std::optional<MetadataType> tgcSamplingFrequency() const {
    return _tgc_sampling_frequency;
  }
  inline void setTgcSamplingFrequency(std::optional<MetadataType> tgcSamplingFrequency) {
    _tgc_sampling_frequency = tgcSamplingFrequency;
  }

  inline std::optional<MetadataType> modulationFrequency() const { return _modulation_frequency; }
  inline void setModulationFrequency(std::optional<MetadataType> modulationFrequency) {
    _modulation_frequency = modulationFrequency;
  }

  // Members
 private:
  // Probes used for this receive setup
  std::weak_ptr<Probe> _probe;

  // Sampling frequency [Hz]
  MetadataType _sampling_frequency = 0.;

  // Number of samples
  uint32_t _number_samples = 0;

  // Type of sampling:
  SAMPLING_TYPE _sampling_type = SAMPLING_TYPE::DIRECT_RF;

  // Maps the RF line with the corresponding element
  // channel_element_index = _channel_mapping[rf_index]
  std::vector<int> _channel_mapping;

  // (Optional) Analog TGC profile sampled at tgc_sampling_frequency [dB]
  std::vector<MetadataType> _tgc_profile = {0.f};

  // (Optional) Sampling frequency of the TGC profile [Hz]
  std::optional<MetadataType> _tgc_sampling_frequency = std::nullopt;

  // (Optional) Modulation frequency used in case of IQ-data [Hz]
  std::optional<MetadataType> _modulation_frequency = std::nullopt;
};

}  // namespace uff
