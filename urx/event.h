#pragma once

#include <iosfwd>
#include <memory>
#include <utility>

#include <urx/receive_setup.h>
#include <urx/transmit_setup.h>
#include <urx/urx.h>

namespace urx {

/**
 * @brief The URX class describing an unique ultrasound event
 * 
 * The event is composed by a single transmit and receive setup
 */
struct Event {
  bool operator==(const Event& other) const = default;

  /// Description of the transmit event (probe/channels, waves, excitations, etc.)
  TransmitSetup transmit_setup;

  /// Description of sampled channel data (probe/channels, sampling, TGC, etc.)
  ReceiveSetup receive_setup;
};

}  // namespace urx
