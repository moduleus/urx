#pragma once

#include <uff/time_offset_base.h>
#include <uff/trigger_location.h>
#include <uff/uff.h>

namespace uff {

struct TriggerOut {
  enum class Polarity { POSITIVE = 0, NEGATIVE = 1 };
  
  auto operator <=>(const TriggerOut& other) const = default;

  // Duration of the pulse [s]
  double _pulse_duration = 0.;

  // Polarity of the trigger
  Polarity _polarity;

  // Location of the destination: where to send the trigger  
  TriggerLocation _destination;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
