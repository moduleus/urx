#pragma once

#include <uff/time_offset_base.h>
#include <uff/uff.h>

namespace uff {

class TriggerOut : public TimeOffsetBase {
 public:
  enum class Polarity { POSITIVE = 0, NEGATIVE = 1 };

  // CTOR & DTOR
  TriggerOut() = default;
  TriggerOut(const MetadataType& pulse_duration, const Polarity& polarity,
             const MetadataType& time_offset = 0.)
      : TimeOffsetBase(time_offset), _pulse_duration(pulse_duration), _polarity(polarity){};
  TriggerOut(const TriggerOut&) = default;
  TriggerOut(TriggerOut&&) = default;
  ~TriggerOut() override = default;

  // Operators
  TriggerOut& operator=(const TriggerOut& other) noexcept = default;
  TriggerOut& operator=(TriggerOut&& other) noexcept = default;
  inline bool operator==(const TriggerOut& other) const {
    return ((_time_offset == other._time_offset) && (_pulse_duration == other._pulse_duration) &&
            (_polarity == other._polarity));
  }
  inline bool operator!=(const TriggerOut& other) const { return !(*this == other); }

  // Accessors
  inline MetadataType pulseDuration() const { return _pulse_duration; }
  inline void setPulseDuration(const MetadataType& pulse_duration) {
    _pulse_duration = pulse_duration;
  }

  inline Polarity polarity() const { return _polarity; }
  inline void setPolarity(const Polarity& polarity) { _polarity = polarity; }

  // Members
 private:
  // Duration of the pulse [s]
  MetadataType _pulse_duration = 0.;

  // Polarity of the trigger
  Polarity _polarity;
};

}  // namespace uff
