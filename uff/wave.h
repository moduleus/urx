#pragma once

#include <uff/aperture.h>
#include <uff/excitation.h>
#include <uff/transform.h>
#include <uff/types.h>
#include <uff/uff.h>

namespace uff {
class Wave {
  // CTOR & DTOR
 public:
  Wave() = default;
  Wave(const Wave&) = default;
  Wave(Wave&&) = default;
  ~Wave() = default;

  // Operators
 public:
  Wave& operator=(const Wave&) noexcept = default;
  Wave& operator=(Wave&& other) noexcept = default;
  bool operator==(const Wave& other) const {
    return ((_origin == other._origin) && (_wave_type == other._wave_type) &&
            (_aperture == other._aperture) && (_excitation == other._excitation));
  }
  inline bool operator!=(const Wave& other) const { return !(*this == other); }

  // Accessors
 public:
  inline const uff::Transform& origin() const { return _origin; }
  inline void setOrigin(const uff::Transform& origin) { _origin = origin; }

  inline const uff::WaveType& waveType() const { return _wave_type; }
  inline void setWaveType(const uff::WaveType& waveType) { _wave_type = waveType; }

  inline const uff::Aperture& aperture() const { return _aperture; }
  inline void setAperture(const uff::Aperture& aperture) { _aperture = aperture; }

  inline const uff::Excitation& excitation() const { return _excitation; }
  inline void setExcitation(const uff::Excitation& excitation) { _excitation = excitation; }

  // Members
 private:
  // Geometric origin of the wave.
  uff::Transform _origin;

  uff::WaveType _wave_type = uff::WaveType::PLANE_WAVE;

  // Description of the aperture used to produce the wave
  uff::Aperture _aperture;

  // [Optional] excitation waveform
  uff::Excitation _excitation;
};

}  // namespace uff
