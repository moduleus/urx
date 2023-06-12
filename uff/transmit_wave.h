#pragma once

#include <uff/uff.h>
#include <uff/wave.h>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
namespace uff {

class TransmitWave {
  // CTOR & DTOR
 public:
  TransmitWave() = default;
  TransmitWave(const TransmitWave&) = default;
  TransmitWave(TransmitWave&&) = default;
  ~TransmitWave() = default;

  // Operators
 public:
  TransmitWave& operator=(const TransmitWave& other) noexcept = default;
  TransmitWave& operator=(TransmitWave&& other) noexcept = default;
  bool operator==(const TransmitWave& other) const {
    return ((_wave.expired() == other._wave.expired()) &&
            (_wave.expired() || (*(_wave.lock()) == *(other._wave.lock()))) &&
            (_time_offset == other._time_offset) && (_weight == other._weight));
  }
  inline bool operator!=(const TransmitWave& other) const { return !(*this == other); }

  // Accessors
 public:
  inline std::weak_ptr<uff::Wave> wave() const { return _wave; }
  inline void setWave(std::weak_ptr<uff::Wave> wave) { _wave = std::move(wave); }

  inline MetadataType timeOffset() const { return _time_offset; }
  inline void setTimeOffset(MetadataType timeOffset) { _time_offset = timeOffset; }

  inline MetadataType weight() const { return _weight; }
  inline void setWeight(MetadataType weight) { _weight = weight; }

  // Members
 private:
  // Reference to the geometry of the wave that was transmitted
  std::weak_ptr<uff::Wave> _wave;

  // Time delay between the start of the event and the moment this wave reaches the closest element in the probe [s]. [Default = 0s]
  MetadataType _time_offset = 0.f;

  // Weight applied to the wave within the event [unitless between -1 and +1]. This may be used to describe pulse inversion sequences. [Default = 1]
  MetadataType _weight = 1.f;
};

}  // namespace uff
