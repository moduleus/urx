#pragma once

#include <memory>

#include <uac/destination_link.h>
#include <uac/hw_config.h>
#include <uac/trigger.h>

#include <urx/detail/double_nan.h>

namespace uac {

struct IGroup {
  virtual ~IGroup() = 0;

  bool operator==(const IGroup& other) const {
    return time_offset == other.time_offset && trigger_in == other.trigger_in &&
           trigger_out == other.trigger_out && repetition_count == other.repetition_count &&
           destinations == other.destinations && period == other.period &&
           hw_config == other.hw_config;
  }

  bool operator!=(const IGroup& other) const { return !operator==(other); }

  urx::DoubleNan time_offset{0};

  TriggerIn trigger_in;

  Trigger trigger_out;

  uint32_t repetition_count = 0;

  std::vector<DestinationLink> destinations;

  urx::DoubleNan period;

  HwConfig hw_config;
};

IGroup::~IGroup(){};

}  // namespace uac
