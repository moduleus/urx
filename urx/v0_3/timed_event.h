#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/event.h>

namespace urx::v0_3 {

struct TimedEvent {
  double time_offset;
  std::shared_ptr<Event> event;
};

}  // namespace urx::v0_3
