#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/timed_event.h>
#include <urx/v0_3/trigger_in.h>
#include <urx/v0_3/trigger_out.h>

namespace urx::v0_3 {

struct Sequence {
  double time_offset;
  std::optional<TriggerIn> trigger_in;
  std::optional<TriggerOut> trigger_out;
  std::vector<TimedEvent> timed_events;
};

}  // namespace urx::v0_3
