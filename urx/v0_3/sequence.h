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
  std::shared_ptr<TriggerIn> trigger_in;
  std::shared_ptr<TriggerOut> trigger_out;
  std::vector<std::shared_ptr<TimedEvent>> timed_events;
};

}  // namespace urx::v0_3
