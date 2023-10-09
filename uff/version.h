#pragma once

#include <uff/uff.h>

#include <cstdint>
#include <string>

namespace uff {

/**
 * @brief The UFF class describing the current version of the UFF architecture
 * 
 * The version follow the standard on software version : major.minor.patch 
 */
struct Version {
  auto operator<=>(const Version& other) const = default;

  /// Major is the first version component
  /// The major is increased when modifications can break the API
  uint16_t major = UFF_VERSION_MAJOR;

  /// Minor is the second version component
  /// The minor is increased when adding new features to the API with backward compatibility
  uint16_t minor = UFF_VERSION_MINOR;

  /// Patch is the third version component
  /// The patch is increased when a minor fix is applied on the code, with backward compatibility
  uint16_t patch = UFF_VERSION_PATCH;
};

}  // namespace uff
