#pragma once

#include <iosfwd>
#include <memory>
#include <utility>

#include <urx/receive_setup.h>
#include <urx/transmit_setup.h>
#include <urx/urx.h>

namespace urx {

struct Event {
  bool operator==(const Event& other) const {
    return transmit_setup == other.transmit_setup && receive_setup == other.receive_setup;
  }

  bool operator!=(const Event& other) const { return !operator==(other); }

  TransmitSetup transmit_setup;

  ReceiveSetup receive_setup;
};

}  // namespace urx
