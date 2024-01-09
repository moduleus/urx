#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/probe.h>
#include <urx/v0_3/wave.h>

namespace urx::v0_3 {

struct TransmitSetup {
  double time_offset;

  std::weak_ptr<Probe> probe;
  std::weak_ptr<Wave> wave;
};

}  // namespace urx::v0_3
