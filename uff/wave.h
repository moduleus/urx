/*!
 * Copyright Moduleus
 * \file uff/wave.h
 * \brief
 */

#ifndef UFF_WAVE_H
#define UFF_WAVE_H

#include <iosfwd>
#include <string>
#include "uff/aperture.h"
#include "uff/excitation.h"
#include "uff/object.h"
#include "uff/transform.h"
#include "uff/types.h"
#include "uff/uff.h"

namespace uff {

/**
 * @brief UFF class to describe the geometry of a transmitted wave or beam
 */
class Wave : public uff::Object {
  UFF_TYPE_MACRO(Wave, uff::Object);

 public:
  Wave() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  const uff::Transform& origin() const { return m_origin; }
  void setOrigin(const uff::Transform& origin) { m_origin = origin; }

  const uff::WaveType& waveType() const { return m_waveType; }
  void setWaveType(const uff::WaveType& waveType) { m_waveType = waveType; }

  const uff::Aperture& aperture() const { return m_aperture; }
  void setAperture(const uff::Aperture& aperture) { m_aperture = aperture; }

  const uff::Excitation& excitation() const { return m_excitation; }
  void setExcitation(const uff::Excitation& excitation) { m_excitation = excitation; }

  bool operator==(const Wave& other) const {
    return ((m_origin == other.m_origin) && (m_waveType == other.m_waveType) &&
            (m_aperture == other.m_aperture) && (m_excitation == other.m_excitation));
  }

  inline bool operator!=(const Wave& other) const { return !(*this == other); }

 private:
  // Geometric origin of the wave.
  uff::Transform m_origin;

  // enumerated type ( [0] -> converging, [1] -> diverging, [2]-> plane, [3]-> cylindrical, [4] -> photoacoustic)
  uff::WaveType m_waveType = uff::WaveType::PLANE_WAVE;

  // Description of the aperture used to produce the wave
  uff::Aperture m_aperture;

  // [Optional] excitation waveform
  uff::Excitation m_excitation;
};

}  // namespace uff

#endif  // UFF_WAVE_H