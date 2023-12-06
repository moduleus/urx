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

struct TransmitSetup {
  bool operator==(const TransmitSetup& other) const = default;

  std::weak_ptr<Probe> probe = std::weak_ptr<Probe>();
  std::weak_ptr<Wave> wave = std::weak_ptr<Wave>();

  Transform probe_transform;

  DoubleNan time_offset{0};
};

}  // namespace urx
