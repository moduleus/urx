#pragma once

#include <uac/excitation.h>
#include <uac/receive_setup.h>
#include <uac/transmit_setup.h>
#include <uac/trigger.h>

#include <urx/detail/double_nan.h>
#include <urx/event.h>

namespace uac {

struct Event : urx::detail::Event<TransmitSetup, ReceiveSetup> {
  bool operator==(const Event& other) const {
    return urx::detail::Event<TransmitSetup, ReceiveSetup>::operator==(other) &&
           time_offset == other.time_offset && trigger_in == other.trigger_in &&
           trigger_out == other.trigger_out && trigger_out == other.trigger_out;
  }

  bool operator!=(const Event& other) const { return !operator==(other); }

  urx::DoubleNan time_offset;

  TriggerIn trigger_in;

  Trigger trigger_out;

  HwConfig hw_config;
};

}  // namespace uac
