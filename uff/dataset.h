#pragma once

#include <uff/acquisition.h>
#include <uff/version.h>

namespace uff {

/**
 * @brief The UFF Dataset class
 */
struct Dataset {
  inline bool operator==(const Dataset& other) const {
    return ((version == other.version) && (acquisition == other.acquisition));
  }
  inline bool operator!=(const Dataset& other) const { return !(*this == other); }

  Version version;

  Acquisition acquisition;
};

}  // namespace uff
