#pragma once

#include <uff/uff.h>

namespace uff {

struct Type {
  enum class Location { EXT = 0, SW = 1 };

  auto operator<=>(const Type& other) const = default;

  Location _location = Location::EXT;

  uint16_t _id = 0;
};

}  // namespace uff
