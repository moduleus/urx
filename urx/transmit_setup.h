#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <urx/detail/compare.h>
#include <urx/detail/double_nan.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/urx.h>
#include <urx/vector.h>
#include <urx/wave.h>

namespace urx {

/**
 * @brief The URX class describing the transmission of the ultrasonic wave
 */
struct TransmitSetup {
  bool operator==(const TransmitSetup& other) const = default;

  /// Reference to the probe use in transmission
  std::weak_ptr<Probe> probe = std::weak_ptr<Probe>();

  /// Location of the probe in space reference for this TransmitSetup
  Transform probe_transform;

  /// Time offset delaying the launch of the whole transmission related to the beginning of the
  DoubleNan time_offset{0};

  /// The wave emitted by this TransmitSetup
  std::weak_ptr<Wave> wave = std::weak_ptr<Wave>();
};

}  // namespace urx
