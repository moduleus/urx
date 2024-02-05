#pragma once

#include <string>

#include <uac/enums.h>

#include <urx/detail/double_nan.h>

namespace uac {

struct Trigger {
  bool operator==(const Trigger& other) const {
    return channel == other.channel && time_offset == other.time_offset &&
           pulse_duration == other.pulse_duration;
  }

  bool operator!=(const Trigger& other) const { return !operator==(other); }

  std::string channel;
  urx::DoubleNan time_offset;
  urx::DoubleNan pulse_duration;
};

struct TriggerIn : Trigger {
  bool operator==(const TriggerIn& other) const {
    return Trigger::operator==(other) && edge == other.edge;
  }

  bool operator!=(const TriggerIn& other) const { return !operator==(other); }

  Edge edge = Edge::UNDEFINED;
};

}  // namespace uac
