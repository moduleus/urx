#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <urx/urx.h>

namespace urx {

/**
 * @brief The URX class describing the current version of the URX architecture
 * 
 * The version follow the standard on software version : major.minor.patch 
 */
struct Version {
  bool operator==(const Version& other) const = default;

  /// Major is the first version component
  /// The major is increased when modifications can break the API
  uint16_t major = URX_VERSION_MAJOR;

  /// Minor is the second version component
  /// The minor is increased when adding new features to the API with backward compatibility
  uint16_t minor = URX_VERSION_MINOR;

  /// Patch is the third version component
  /// The patch is increased when a minor fix is applied on the code, with backward compatibility
  uint16_t patch = URX_VERSION_PATCH;
};

}  // namespace urx
