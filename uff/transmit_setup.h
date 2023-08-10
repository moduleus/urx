#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <uff/aperture.h>
#include <uff/coordinates.h>
#include <uff/excitation.h>
#include <uff/probe.h>
#include <uff/time_offset_base.h>
#include <uff/transform.h>
#include <uff/types.h>
#include <uff/uff.h>

namespace uff {

class TransmitSetup : public TimeOffsetBase {
 public:
  // CTOR & DTOR
  TransmitSetup() = delete;
  explicit TransmitSetup(std::weak_ptr<Probe> probe, MetadataType time_offset = 0.)
      : TimeOffsetBase(time_offset), _probe(std::move(probe)) {}
  TransmitSetup(const TransmitSetup&) = default;
  TransmitSetup(TransmitSetup&&) = default;
  ~TransmitSetup() override = default;

  // Operators
  TransmitSetup& operator=(const TransmitSetup& other) noexcept = default;
  TransmitSetup& operator=(TransmitSetup&& other) noexcept = default;
  bool operator==(const TransmitSetup& other) const {
    bool are_channel_excitations_equaled =
        _channel_excitations.size() == other._channel_excitations.size();
    for (uint32_t i = 0; i < _channel_excitations.size() && are_channel_excitations_equaled; ++i) {
      are_channel_excitations_equaled =
          are_channel_excitations_equaled &&
          (_channel_excitations[i].expired() == other._channel_excitations[i].expired()) &&
          (_channel_excitations[i].expired() ||
           (*(_channel_excitations[i].lock()) == *(other._channel_excitations[i].lock())));
    }

    return (TimeOffsetBase::operator==(other) && (_probe.expired() == other._probe.expired()) &&
            (_probe.expired() || (*(_probe.lock()) == *(other._probe.lock()))) &&
            (_origin == other._origin) && (_wave_type == other._wave_type) &&
            (_aperture == other._aperture) &&
            (_time_zero_reference_point == other._time_zero_reference_point) &&
            (_time_zero == other._time_zero) && (_channel_mapping == other._channel_mapping) &&
            are_channel_excitations_equaled && (_channel_delays == other._channel_delays));
  }
  inline bool operator!=(const TransmitSetup& other) const { return !(*this == other); }

  // Accessors
  inline std::weak_ptr<Probe> probe() const { return _probe; }
  inline void setProbe(const std::weak_ptr<Probe>& probe) { _probe = probe; }

  inline const Transform& origin() const { return _origin; }
  inline void setOrigin(const Transform& origin) { _origin = origin; }

  inline const WaveType& waveType() const { return _wave_type; }
  inline void setWaveType(const WaveType& waveType) { _wave_type = waveType; }

  inline const std::optional<Aperture>& aperture() const { return _aperture; }
  inline void setAperture(const std::optional<Aperture>& aperture) { _aperture = aperture; }

  inline Coordinates3D<MetadataType> timeZeroReferencePoint() const {
    return _time_zero_reference_point;
  }
  inline void setTimeZeroReferencePoint(
      const Coordinates3D<MetadataType>& time_zero_reference_point) {
    _time_zero_reference_point = time_zero_reference_point;
  }

  inline MetadataType timeZero() const { return _time_zero; }
  inline void setTimeZero(MetadataType time_zero) { _time_zero = time_zero; }

  inline const std::vector<int>& channelMapping() const { return _channel_mapping; }
  inline void setChannelMapping(const std::vector<int>& channel_mapping) {
    _channel_mapping = channel_mapping;
  }

  inline const std::vector<std::weak_ptr<Excitation>>& channelExcitations() const {
    return _channel_excitations;
  }
  inline void setChannelExcitations(
      const std::vector<std::weak_ptr<Excitation>>& channel_excitations) {
    _channel_excitations = channel_excitations;
  }

  inline const std::vector<MetadataType>& channelDelays() const { return _channel_delays; }
  inline void setChannelDelays(const std::vector<MetadataType>& channel_delays) {
    _channel_delays = channel_delays;
  }

  // Members
 private:
  // Reference to the probe use in transmission
  std::weak_ptr<Probe> _probe;

  // Geometric origin of the wave.
  Transform _origin;

  // Type of wave
  WaveType _wave_type = WaveType::UNDEFINED;

  // Description of the aperture used to produce the wave
  std::optional<Aperture> _aperture = std::nullopt;

  // Time zero reference point in Probe orthogonal coordinate system
  Coordinates3D<MetadataType> _time_zero_reference_point = Coordinates3D<MetadataType>{0, 0, 0};

  // Time zero for the wave [s]
  MetadataType _time_zero = 0.;

  // Map of channels to transducer elements
  // Each value corresponds to one probe channel index
  std::vector<int> _channel_mapping;

  // List of the excitation waveform for each channel
  std::vector<std::weak_ptr<Excitation>> _channel_excitations;

  // List of the excitation waveform for each channel
  std::vector<MetadataType> _channel_delays;
};

}  // namespace uff
