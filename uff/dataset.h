#pragma once

#include <memory>

#include <uff/acquisition.h>
#include <uff/version.h>

namespace uff {

/**
 * @brief The UFF class heading the entire UFF file/architecture
 * 
 * This class is the header class that references the acquisition and its version
 */
struct Dataset {
  bool operator==(const Dataset& other) const = default;

  /// Uff version of this dataset
  Version version;

  /// Whole acquisition information
  Acquisition acquisition;
};

}  // namespace uff
