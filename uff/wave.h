#pragma once

#include <uff/aperture.h>
#include <uff/excitation.h>
#include <uff/point.h>
#include <uff/transform.h>
#include <uff/types.h>
#include <uff/uff.h>

#include <optional>

namespace uff {
class Wave {
 public:
  // CTOR & DTOR
  Wave() = default;
  Wave(const Wave&) = default;
  Wave(Wave&&) = default;
  ~Wave() = default;

  // Operators
  Wave& operator=(const Wave&) noexcept = default;
  Wave& operator=(Wave&& other) noexcept = default;
  bool operator==(const Wave& other) const {
    bool are_channel_excitations_equaled =
        _channel_excitations.size() == other._channel_excitations.size();
    for (uint32_t i = 0; i < _channel_excitations.size() && are_channel_excitations_equaled; ++i) {
      are_channel_excitations_equaled =
          are_channel_excitations_equaled &&
          (_channel_excitations[i].expired() == other._channel_excitations[i].expired()) &&
          (_channel_excitations[i].expired() ||
           (*(_channel_excitations[i].lock()) == *(other._channel_excitations[i].lock())));
    }

    return ((_origin == other._origin) && (_wave_type == other._wave_type) &&
            (_aperture == other._aperture) &&
            (_time_zero_reference_point == other._time_zero_reference_point) &&
            (_time_zero == other._time_zero) && (_channel_mapping == other._channel_mapping) &&
            are_channel_excitations_equaled && (_channel_delays == other._channel_delays));
  }
  inline bool operator!=(const Wave& other) const { return !(*this == other); }

  // Accessors
  inline const Transform& origin() const { return _origin; }
  inline void setOrigin(const Transform& origin) { _origin = origin; }

  inline const WaveType& waveType() const { return _wave_type; }
  inline void setWaveType(const WaveType& waveType) { _wave_type = waveType; }

  inline const std::optional<Aperture>& aperture() const { return _aperture; }
  inline void setAperture(const std::optional<Aperture>& aperture) { _aperture = aperture; }

  inline Point3D<MetadataType> timeZeroReferencePoint() const { return _time_zero_reference_point; }
  inline void setTimeZeroReferencePoint(const Point3D<MetadataType>& time_zero_reference_point) {
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
  // Geometric origin of the wave.
  Transform _origin;

  WaveType _wave_type = WaveType::UNDEFINED;

  // Description of the aperture used to produce the wave
  std::optional<Aperture> _aperture = std::nullopt;

  // Time zero reference point in Probe orthogonal coordinate system
  Point3D<MetadataType> _time_zero_reference_point = Point3D<MetadataType>{0, 0, 0};

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
