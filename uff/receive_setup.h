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
class ReceiveSetup {
 public:
  enum class SAMPLING_TYPE { DIRECT_RF = 0, QUADRATURE_4X_F0 = 1, QUADRATURE_2X_F0 = 2, IQ = 3 };

  // CTOR & DTOR
 public:
  ReceiveSetup() = default;
  ReceiveSetup(const ReceiveSetup&) = default;
  ReceiveSetup(ReceiveSetup&&) = default;
  ~ReceiveSetup() = default;

  // Operators
 public:
  ReceiveSetup& operator=(const ReceiveSetup& other) noexcept = default;
  ReceiveSetup& operator=(ReceiveSetup&& other) noexcept = default;
  inline bool operator==(const ReceiveSetup& other) const {
    return ((_probe.expired() == other._probe.expired()) &&
            (_probe.expired() || (*(_probe.lock()) == *(other._probe.lock()))) &&
            (_time_offset == other._time_offset) &&
            (_sampling_frequency == other._sampling_frequency) &&
            (_sampling_type == other._sampling_type) &&
            (_channel_mapping == other._channel_mapping) && (_tgc_profile == other._tgc_profile) &&
            (_tgc_sampling_frequency == other._tgc_sampling_frequency) &&
            (_modulation_frequency == other._modulation_frequency));
  }
  inline bool operator!=(const ReceiveSetup& other) const { return !(*this == other); }

  // Accessors
 public:
  inline std::weak_ptr<uff::Probe> probe() const { return _probe; }
  inline void setProbe(std::weak_ptr<uff::Probe> probe) { _probe = std::move(probe); }

  inline MetadataType timeOffset() const { return _time_offset; }
  inline void setTimeOffset(const MetadataType& timeOffset) { _time_offset = timeOffset; }

  inline MetadataType samplingFrequency() const { return _sampling_frequency; }
  inline void setSamplingFrequency(const MetadataType& samplingFrequency) {
    _sampling_frequency = samplingFrequency;
  }

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
  std::weak_ptr<uff::Probe> _probe;

  // Time before the first samples [s]
  MetadataType _time_offset = 0.f;

  // Sampling frequency [Hz]
  MetadataType _sampling_frequency = 0.f;

  // Type of sampling:
  SAMPLING_TYPE _sampling_type = SAMPLING_TYPE::DIRECT_RF;

  // Maps the RF line with the corresponding element
  // channel_element_index = _channel_mapping[rf_index]
  std::vector<int> _channel_mapping;

  // (Optional) Analog TGC profile sampled at tgc_sampling_frequency [dB]
  std::vector<MetadataType> _tgc_profile = {0.f};

  //     (Optional) Sampling frequency of the TGC profile [Hz]
  std::optional<MetadataType> _tgc_sampling_frequency = std::nullopt;

  //     (Optional) Modulation frequency used in case of IQ-data [Hz]
  std::optional<MetadataType> _modulation_frequency = std::nullopt;
};

}  // namespace uff
