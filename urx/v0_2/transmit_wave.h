// Copyright Moduleus

#ifndef URX_TRANSMIT_WAVE_H
#define URX_TRANSMIT_WAVE_H

#include <iosfwd>
#include <memory>
#include <string>
#include <utility>

#include <urx/v0_2/object.h>
#include <urx/v0_2/urx.h>
#include <urx/v0_2/wave.h>

namespace urx::v0_2 {

/**
 * @brief The URX TransmitWave class
 */
class TransmitWave : public Object {
  URX_TYPE_MACRO(TransmitWave, Object);

 public:
  TransmitWave() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  /* */
  std::weak_ptr<Wave> wave() const { return m_wave; }
  void setWave(std::weak_ptr<Wave> wave) { m_wave = std::move(wave); }

  MetadataType timeOffset() const { return m_timeOffset; }
  void setTimeOffset(MetadataType timeOffset) { m_timeOffset = timeOffset; }

  MetadataType weight() const { return m_weight; }
  void setWeight(MetadataType weight) { m_weight = weight; }

  bool operator==(const TransmitWave& other) const {
    return ((m_wave.expired() == other.m_wave.expired()) &&
            (m_wave.expired() || (*(m_wave.lock()) == *(other.m_wave.lock()))) &&
            (m_timeOffset == other.m_timeOffset) && (m_weight == other.m_weight));
  }

  inline bool operator!=(const TransmitWave& other) const { return !(*this == other); }

 private:
  // Reference to the geometry of the wave that was transmitted
  std::weak_ptr<Wave> m_wave;

  // Time delay between the start of the event and the moment this wave reaches the closest element in the probe [s]. [Default = 0s]
  MetadataType m_timeOffset = 0.f;

  // Weight applied to the wave within the event [unitless between -1 and +1]. This may be used to describe pulse inversion sequences. [Default = 1]
  MetadataType m_weight = 1.f;
};

}  // namespace urx::v0_2

#endif  // URX_TRANSMIT_WAVE_H
