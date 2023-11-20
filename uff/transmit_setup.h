#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <uff/probe.h>
#include <uff/transform.h>
#include <uff/uff.h>
#include <uff/vector.h>
#include <uff/wave.h>

namespace uff {

/**
 * @brief The UFF class describing the transmission of the ultrasonic wave
 */
struct TransmitSetup {
  bool operator==(const TransmitSetup& other) const {
    return (is_nan_or_equal(time_offset, other.time_offset) &&
            (probe.expired() == other.probe.expired()) &&
            (probe.expired() || *(probe.lock()) == *(other.probe.lock())) &&
            (probe_transform == other.probe_transform) &&
            (wave.expired() == other.wave.expired()) &&
            (wave.expired() || *(wave.lock()) == *(other.wave.lock())));
  }
  inline bool operator!=(const TransmitSetup& other) const { return !(*this == other); }

  /// Reference to the probe use in transmission
  std::weak_ptr<Probe> probe = std::weak_ptr<Probe>();

  /// Location of the probe in space reference for this TransmitSetup
  Transform probe_transform;

  /// Time offset delaying the launch of the whole transmission related to the beginning of the
  double time_offset = UFF_NAN;

  /// The wave emitted by this TransmitSetup
  std::weak_ptr<Wave> wave = std::weak_ptr<Wave>();
};

}  // namespace uff
