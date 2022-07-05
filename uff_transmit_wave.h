/*!
 * Copyright Moduleus
 * \file uff_transmit_wave.h
 * \brief
 */

#ifndef UFF_TRANSMIT_WAVE_H
#define UFF_TRANSMIT_WAVE_H

// UFF
#include "uff_object.h"
#include "uff_wave.h"

#include <iostream>
#include <memory>

namespace uff {

/**
 * @brief The UFF TransmitWave class
 */
class TransmitWave : public uff::Object {
  UFF_TYPE_MACRO(TransmitWave, uff::Object);

 public:
  TransmitWave() {}

  void printSelf(std::ostream& os, std::string indent) const override;

  /* */
  std::weak_ptr<uff::Wave> wave() const { return m_wave; }
  void setWave(std::weak_ptr<uff::Wave> wave) { m_wave = wave; }

  FloatingType timeOffset() const { return m_timeOffset; }
  void setTimeOffset(FloatingType timeOffset) { m_timeOffset = timeOffset; }

  FloatingType weight() const { return m_weight; }
  void setWeight(FloatingType weight) { m_weight = weight; }

  bool operator==(const TransmitWave& other) const {
    return ((m_wave.expired() == other.m_wave.expired()) &&
            (m_wave.expired() || (*(m_wave.lock()) == *(other.m_wave.lock()))) &&
            (m_timeOffset == other.m_timeOffset) && (m_weight == other.m_weight));
  }

  inline bool operator!=(const TransmitWave& other) const { return !(*this == other); }

 private:
  // Reference to the geometry of the wave that was transmitted
  std::weak_ptr<uff::Wave> m_wave;

  // Time delay between the start of the event and the moment this wave reaches the closest element in the probe [s]. [Default = 0s]
  FloatingType m_timeOffset = 0.f;

  // Weight applied to the wave within the event [unitless between -1 and +1]. This may be used to describe pulse inversion sequences. [Default = 1]
  FloatingType m_weight = 1.f;
};

}  // namespace uff

#endif  // UFF_TRANSMIT_WAVE_H
