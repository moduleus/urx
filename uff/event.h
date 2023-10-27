#pragma once

#include <iosfwd>
#include <memory>
#include <utility>

#include <uff/receive_setup.h>
#include <uff/transmit_setup.h>
#include <uff/uff.h>

namespace uff {

/**
 * @brief The UFF class describing an unique ultrasound event
 * 
 * The event is composed by a single transmit and receive setup
 */
struct Event {
  bool operator==(const Event& other) const {
    return (transmit_setup == other.transmit_setup) &&
           (receive_setup == other.receive_setup);
  }
  inline bool operator!=(const Event& other) const { return !(*this == other); }

  /// Description of the transmit event (probe/channels, waves, excitations, etc.)
  std::shared_ptr<TransmitSetup> transmit_setup;

  /// Description of sampled channel data (probe/channels, sampling, TGC, etc.)
  std::shared_ptr<ReceiveSetup> receive_setup;
};

}  // namespace uff
