#pragma once

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/polarity.h>
#include <urx/v0_3/trigger_destination.h>

namespace urx::v0_3 {

struct TriggerOut {
  double pulse_duration;
  TriggerDestination destination;
  Polarity polarity;
};

}  // namespace urx::v0_3
