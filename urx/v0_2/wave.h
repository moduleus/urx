// Copyright Moduleus

#ifndef URX_WAVE_H
#define URX_WAVE_H

#include <iosfwd>
#include <string>

#include <urx/v0_2/aperture.h>
#include <urx/v0_2/excitation.h>
#include <urx/v0_2/object.h>
#include <urx/v0_2/transform.h>
#include <urx/v0_2/types.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief URX class to describe the geometry of a transmitted wave or beam
 */
class Wave : public Object {
  URX_TYPE_MACRO(Wave, Object);

 public:
  Wave() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  const Transform& origin() const { return m_origin; }
  void setOrigin(const Transform& origin) { m_origin = origin; }

  const WaveType& waveType() const { return m_waveType; }
  void setWaveType(const WaveType& waveType) { m_waveType = waveType; }

  const Aperture& aperture() const { return m_aperture; }
  void setAperture(const Aperture& aperture) { m_aperture = aperture; }

  const Excitation& excitation() const { return m_excitation; }
  void setExcitation(const Excitation& excitation) { m_excitation = excitation; }

  bool operator==(const Wave& other) const {
    return ((m_origin == other.m_origin) && (m_waveType == other.m_waveType) &&
            (m_aperture == other.m_aperture) && (m_excitation == other.m_excitation));
  }

  inline bool operator!=(const Wave& other) const { return !(*this == other); }

 private:
  // Geometric origin of the wave.
  Transform m_origin;

  // enumerated type ( [0] -> converging, [1] -> diverging, [2]-> plane, [3]-> cylindrical, [4] -> photoacoustic)
  WaveType m_waveType = WaveType::PLANE_WAVE;

  // Description of the aperture used to produce the wave
  Aperture m_aperture;

  // [Optional] excitation waveform
  Excitation m_excitation;
};

}  // namespace urx::v0_2

#endif  // URX_WAVE_H