#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/probe.h>
#include <urx/v0_3/wave.h>

namespace urx::v0_3 {

struct TransmitSetup {
  std::shared_ptr<Probe> probe;
  std::shared_ptr<Wave> wave;
};

}  // namespace urx::v0_3
