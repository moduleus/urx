#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <uff/probe.h>
#include <uff/transform.h>
#include <uff/types.h>
#include <uff/uff.h>
#include <uff/wave.h>
#include <uff/vector.h>

namespace uff {

/**
 * @brief The UFF class describing the transmission of the ultrasonic wave
 */
struct TransmitSetup {
  bool operator==(const TransmitSetup& other) const {
    return ((time_offset == other.time_offset) && (probe == other.probe) &&
            (probe_transform == other.probe_transform) &&
            (wave == other.wave));
  }
  inline bool operator!=(const TransmitSetup& other) const { return !(*this == other); }

  /// Reference to the probe use in transmission
  std::shared_ptr<Probe> probe;

  /// Location of the probe in space reference for this TransmitSetup
  std::optional<Transform> probe_transform = std::nullopt;

  /// Time offset delaying the launch of the whole transmission related to the beginning of the
  double time_offset = 0.;

  /// The wave emitted by this TransmitSetup
  std::shared_ptr<Wave> wave;
};

}  // namespace uff
