#pragma once

#include <uff/uff.h>

namespace uff {

class TimeOffsetBase {
 public:
  // CTOR & DTOR
  TimeOffsetBase() = default;
  TimeOffsetBase(const TimeOffsetBase&) = default;
  TimeOffsetBase(TimeOffsetBase&&) = default;
  explicit TimeOffsetBase(double time_offset) : _time_offset(time_offset){};
  virtual ~TimeOffsetBase() = default;

  // Operators
  TimeOffsetBase& operator=(const TimeOffsetBase& other) noexcept = default;
  TimeOffsetBase& operator=(TimeOffsetBase&& other) noexcept = default;
  bool operator==(const TimeOffsetBase& other) const { return _time_offset == other._time_offset; }
  inline bool operator!=(const TimeOffsetBase& other) const { return !(*this == other); }

  // Accessors
  // Time offset
  inline double timeOffset() const { return _time_offset; }
  inline void setTimeOffset(double time_offset) { _time_offset = time_offset; }

  // Members
 protected:
  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
