#pragma once

#include <uff/destination_link.h>
#include <uff/igroup.h>
#include <uff/sequence.h>
#include <uff/uff.h>

namespace uff {

struct Group : public IGroup {
  bool operator==(const Group& other) const {
    return (IGroup::operator==(other) && repetition_rate == other.repetition_rate &&
            sequence == other.sequence);
  }
  inline bool operator!=(const Group& other) const { return !(*this == other); }

  Sequence sequence;

  double repetition_rate = 0.;
};

}  // namespace uff
