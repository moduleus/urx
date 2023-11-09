#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <uff/probe.h>
#include <uff/transform.h>
#include <uff/types.h>
#include <uff/uff.h>
#include <uff/vector.h>
#include <uff/wave.h>

namespace uff {

/**
 * @brief The UFF class describing the transmission of the ultrasonic wave
 */
struct TransmitSetup {
  bool operator==(const TransmitSetup& other) const {
    return (is_nan_or_equal(time_offset, other.time_offset) && (probe == other.probe) &&
            (probe_transform == other.probe_transform) && (wave == other.wave));
  }
  inline bool operator!=(const TransmitSetup& other) const { return !(*this == other); }

  /// Reference to the probe use in transmission
  Probe* probe = nullptr;

  /// Location of the probe in space reference for this TransmitSetup
  Transform probe_transform;

  /// Time offset delaying the launch of the whole transmission related to the beginning of the
  double time_offset = UFF_NAN;

  /// The wave emitted by this TransmitSetup
  Wave* wave = nullptr;
};

}  // namespace uff
