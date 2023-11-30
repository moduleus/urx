#pragma once

#include <memory>

#include <urx/acquisition.h>
#include <urx/version.h>

namespace urx {

/**
 * @brief The URX class heading the entire URX file/architecture
 * 
 * This class is the header class that references the acquisition and its version
 */
struct Dataset {
  bool operator==(const Dataset& other) const = default;

  /// Urx version of this dataset
  Version version;

  /// Whole acquisition information
  Acquisition acquisition;
};

}  // namespace urx
