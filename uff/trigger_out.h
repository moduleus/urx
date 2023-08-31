#pragma once

#include <uff/trigger_location.h>
#include <uff/uff.h>

namespace uff {

struct TriggerOut {
  enum class Polarity { POSITIVE = 0, NEGATIVE = 1, UNDEFINED = -1 };

  auto operator<=>(const TriggerOut& other) const = default;

  // Duration of the pulse [s]
  double pulse_duration = 0.;

  // Polarity of the trigger
  Polarity polarity = Polarity::UNDEFINED;

  // Location of the destination: where to send the trigger
  TriggerLocation destination;

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff
