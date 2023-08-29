#pragma once

#include <uff/uff.h>

namespace uff {

struct TriggerLocation {
  enum class Location { EXT = 0, SW = 1};

  TriggerLocation() = default;
  TriggerLocation(const Location& location, uint16_t id) : _location(location), _id(id){};
  TriggerLocation(const TriggerLocation&) = default;
  TriggerLocation(TriggerLocation&&) = default;
  ~TriggerLocation() = default;

  TriggerLocation& operator=(const TriggerLocation& other) noexcept = default;
  TriggerLocation& operator=(TriggerLocation&& other) noexcept = default;
  inline bool operator==(const TriggerLocation& other) const {
    return ((_location == other._location) && (_id == other._id));
  }
  inline bool operator!=(const TriggerLocation& other) const { return !(*this == other); }

  Location _location = Location::EXT;

  uint16_t _id = 0;
};

}  // namespace uff
