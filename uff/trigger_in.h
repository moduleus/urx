#pragma once

#include <uff/uff.h>
#include <uff/trigger_location.h>

#include <optional>

namespace uff {

struct TriggerIn {
 public:
  enum class TriggerType { RISING = 0, FALLING = 1, HIGH = 2, LOW = 3 };

  // CTOR & DTOR
  TriggerIn() = default;
  TriggerIn(const std::optional<TriggerLocation>& source, const TriggerType& edge) : _source(source), _edge(edge){};
  TriggerIn(const TriggerIn&) = default;
  TriggerIn(TriggerIn&&) = default;
  ~TriggerIn() = default;

  // Operators
  TriggerIn& operator=(const TriggerIn& other) noexcept = default;
  TriggerIn& operator=(TriggerIn&& other) noexcept = default;
  inline bool operator==(const TriggerIn& other) const {
    return ((_source == other._source) && (_edge == other._edge));
  }
  inline bool operator!=(const TriggerIn& other) const { return !(*this == other); }

  // Accessors
  inline std::optional<TriggerLocation> source() const { return _source; }
  inline void setSource(const std::optional<TriggerLocation>& source) { _source = source; }

  inline TriggerType edge() const { return _edge; }
  inline void setEdge(const TriggerType& edge) { _edge = edge; }

  // nullopt means that by default the source of the trigger is END_OF_GROUP
  std::optional<TriggerLocation> _source = std::nullopt;

  TriggerType _edge;
};

}  // namespace uff
