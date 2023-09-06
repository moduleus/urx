#pragma once

#include <uff/destination_link.h>
#include <uff/igroup.h>
#include <uff/sequence.h>
#include <uff/uff.h>

namespace uff {

struct Group : public IGroup {
  enum class SamplingType { DIRECT_RF = 0, IQ = 1 };

  bool operator==(const Group& other) const {
    return (IGroup::operator==(other) && sampling_type == other.sampling_type &&
            repetition_rate == other.repetition_rate && sequence == other.sequence);
  }
  inline bool operator!=(const Group& other) const { return !(*this == other); }

  // Type of sampling:
  SamplingType sampling_type = SamplingType::DIRECT_RF;

  Sequence sequence;

  double repetition_rate = 0.;
};

}  // namespace uff
