#pragma once

#include <memory>
#include <string>
#include <vector>

#include <urx/v0_3/igroup.h>
#include <urx/v0_3/sequence.h>

namespace urx::v0_3 {

struct Group : public IGroup {
  Sequence sequence;
  double repetition_rate;
};

}  // namespace urx::v0_3
