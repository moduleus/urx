#pragma once

#include <memory>
#include <vector>

#include <uff/probe.h>
#include <uff/time_offset_base.h>
#include <uff/uff.h>
#include <uff/wave.h>

namespace uff {

class TransmitSetup : public TimeOffsetBase {
 public:
  // CTOR & DTOR
  TransmitSetup() = delete;
  TransmitSetup(const std::weak_ptr<Probe>& probe, const std::weak_ptr<Wave>& wave,
                MetadataType time_offset = 0.)
      : TimeOffsetBase(time_offset), _probe(probe), _wave(wave) {}
  TransmitSetup(const TransmitSetup&) = default;
  TransmitSetup(TransmitSetup&&) = default;
  ~TransmitSetup() override = default;

  // Operators
  TransmitSetup& operator=(const TransmitSetup& other) noexcept = default;
  TransmitSetup& operator=(TransmitSetup&& other) noexcept = default;
  bool operator==(const TransmitSetup& other) const {
    return (TimeOffsetBase::operator==(other) && (_probe.expired() == other._probe.expired()) &&
            (_probe.expired() || (*(_probe.lock()) == *(other._probe.lock()))) &&
            (_wave.expired() == other._wave.expired()) &&
            (_wave.expired() || (*(_wave.lock()) == *(other._wave.lock()))));
  }
  inline bool operator!=(const TransmitSetup& other) const { return !(*this == other); }

  // Accessors
  inline std::weak_ptr<Probe> probe() const { return _probe; }
  inline void setProbe(std::weak_ptr<Probe> probe) { _probe = probe; }

  inline std::weak_ptr<Wave> wave() const { return _wave; }
  inline void setWave(const std::weak_ptr<Wave>& wave) { _wave = wave; }

  // Members
 private:
  // Reference to the probe use in transmission
  std::weak_ptr<Probe> _probe;

  // List of transmit waves used in this event with their respective time offset and weight
  std::weak_ptr<Wave> _wave;
};

}  // namespace uff
