#pragma once

#include <uff/acquisition.h>
#include <uff/version.h>

namespace uff {

/**
 * @brief The UFF class heading the entire UFF file/architecture
 * 
 * This class is the header class that references the acquisition and its version
 */
struct Dataset {
  inline bool operator==(const Dataset& other) const {
    return ((version == other.version) && (acquisition == other.acquisition));
  }
  inline bool operator!=(const Dataset& other) const { return !(*this == other); }

  /// Uff version of this dataset
  Version version;

  /// Whole acquisition information
  Acquisition acquisition;
};

}  // namespace uff
