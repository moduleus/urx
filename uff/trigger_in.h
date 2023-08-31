#pragma once

#include <uff/trigger_location.h>
#include <uff/uff.h>

#include <optional>

namespace uff {

struct TriggerIn {
  enum class Edge { RISING = 0, FALLING = 1, HIGH = 2, LOW = 3, UNDEFINED = -1 };

  auto operator<=>(const TriggerIn& other) const = default;

  TriggerLocation source;

  Edge edge = Edge::UNDEFINED;
};

}  // namespace uff
