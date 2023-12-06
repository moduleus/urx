#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/receive_setup.h>
#include <urx/v0_3/transmit_setup.h>
#include <urx/v0_3/trigger_in.h>
#include <urx/v0_3/trigger_out.h>

namespace urx::v0_3 {

struct Event {
  TransmitSetup transmit_setup;
  ReceiveSetup receive_setup;
  std::optional<TriggerIn> trigger_in;
  std::optional<TriggerOut> trigger_out;
};

}  // namespace urx::v0_3
