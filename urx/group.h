#pragma once

#include <memory>

#include <urx/event.h>
#include <urx/urx.h>

namespace urx {

/**
 * @brief The URX class describing a Group acquisition
 * 
 * The group is composed of Sequences. 
 * All sequences are the same and are repeated for a certain amount of time.
 */
struct Group {
  enum class SamplingType { RF = 0, IQ = 1, UNDEFINED = -1 };
  enum class DataType { INT16 = 0, INT32 = 1, FLOAT = 2, DOUBLE = 3, UNDEFINED = -1 };

  bool operator==(const Group& other) const {
    return sampling_type == other.sampling_type && data_type == other.data_type &&
           description == other.description && sequence == other.sequence;
  }

  bool operator!=(const Group& other) const { return !operator==(other); }

  SamplingType sampling_type = SamplingType::UNDEFINED;
  DataType data_type = DataType::UNDEFINED;

  std::string description;

  std::vector<Event> sequence;
};

}  // namespace urx
