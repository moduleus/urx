#pragma once

#include <uff/event.h>
#include <uff/uff.h>

namespace uff {

/**
 * @brief The UFF class describing a Group acquisition
 * 
 * The group is composed of Sequences. 
 * All sequences are the same and are repeated for a certain amount of time.
 */
struct Group {
  enum class SamplingType { RF = 0, IQ = 1, UNDEFINED = -1 };
  enum class DataType { INT16 = 0, INT32 = 1, FLOAT = 2, DOUBLE = 3, UNDEFINED = -1 };

  bool operator==(const Group& other) const {
    return (sampling_type == other.sampling_type && data_type == other.data_type &&
            sequence == other.sequence && description == other.description);
  }
  inline bool operator!=(const Group& other) const { return !(*this == other); }

  /// Type of sampling
  SamplingType sampling_type = SamplingType::UNDEFINED;

  /// Data type contained in the raw_data array
  DataType data_type = DataType::UNDEFINED;

  /// List of all event composing an sequence
  /// The number of sequence can be determined from GroupData or you can use Uff_Utils to get it
  std::vector<Event> sequence;

  /// Description of what should do the group acquisition
  std::string description;
};

}  // namespace uff
