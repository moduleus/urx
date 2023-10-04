#pragma once

#include <uff/event.h>
#include <uff/uff.h>

namespace uff {

struct Group {
  enum class SamplingType { DIRECT_RF = 0, IQ = 1 };

  bool operator==(const Group& other) const {
    return (sampling_type == other.sampling_type &&
            sequence == other.sequence &&
            description == other.description);
  }
  inline bool operator!=(const Group& other) const { return !(*this == other); }

  // Type of sampling:
  SamplingType sampling_type = SamplingType::DIRECT_RF;

  std::vector<Event> sequence;

  std::string description;
};

}  // namespace uff
