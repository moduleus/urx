#pragma once

#include <uff/uff.h>

namespace uff {

struct TriggerLocation {
  enum class Type { EXT = 0, SW = 1, UNDEFINED = -1 };

  auto operator<=>(const TriggerLocation& other) const = default;

  Type location = Type::UNDEFINED;

  uint16_t id = 0xFFFF;
};

}  // namespace uff
