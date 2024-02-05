#pragma once

#include <memory>
#include <vector>

#include <uac/detail/compare.h>
#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/hw_config.h>
#include <uac/igroup.h>
#include <uac/super_group.h>
#include <uac/trigger.h>

#include <urx/acquisition.h>
#include <urx/detail/double_nan.h>

namespace uac {

struct Acquisition : urx::detail::Acquisition<Excitation, Group> {
  bool operator==(const Acquisition& other) const {
    return urx::detail::Acquisition<Excitation, Group>::operator==(other) &&
           initial_group == other.initial_group && super_groups == other.super_groups &&
           time_offset == other.time_offset && trigger_in == other.trigger_in &&
           trigger_out == other.trigger_out && hw_config == other.hw_config;
  }

  bool operator!=(const Acquisition& other) const { return !operator==(other); }

  std::weak_ptr<IGroup> initial_group;

  std::vector<SuperGroup> super_groups;

  urx::DoubleNan time_offset{0};

  TriggerIn trigger_in;

  Trigger trigger_out;

  HwConfig hw_config;
};

}  // namespace uac
