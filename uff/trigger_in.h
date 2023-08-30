#pragma once

#include <uff/trigger_location.h>
#include <uff/uff.h>

#include <optional>

namespace uff {

struct TriggerIn {
 public:
  enum class Edge { RISING = 0, FALLING = 1, HIGH = 2, LOW = 3 };

  // CTOR & DTOR
  TriggerIn() = default;
  TriggerIn(const std::optional<Type>& source, const Edge& edge)
      : _source(source), _edge(edge){};
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
  inline std::optional<Type> source() const { return _source; }
  inline void setSource(const std::optional<Type>& source) { _source = source; }

  inline Edge edge() const { return _edge; }
  inline void setEdge(const Edge& edge) { _edge = edge; }

  // nullopt means that by default the source of the trigger is END_OF_GROUP
  std::optional<Type> _source = std::nullopt;

  Edge _edge;
};

}  // namespace uff
