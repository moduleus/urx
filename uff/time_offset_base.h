#pragma once

#include <uff/uff.h>
#include <compare>

namespace uff {

struct TimeOffsetBase {
  auto operator<=>(const TimeOffsetBase& other) const = default;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
