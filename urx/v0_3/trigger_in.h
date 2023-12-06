#pragma once

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/trigger_source.h>
#include <urx/v0_3/trigger_type.h>

namespace urx::v0_3 {

struct TriggerIn {
  std::vector<TriggerSource> sources;
  TriggerType edge;
};

}  // namespace urx::v0_3
