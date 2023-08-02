#pragma once

#include <uff/uff.h>

namespace uff {

class TriggerIn {
 public:
  enum class TriggerSource { END_OF_GROUP = 0, EXT1 = 1, SW1 = 2, SW2 = 3, SW3 = 4 };
  enum class TriggerType { RISING = 0, FALLING = 1, HIGH = 2, LOW = 3 };

 public:
  // CTOR & DTOR
  TriggerIn() = default;
  TriggerIn(const TriggerSource& source, const TriggerType& edge) : _source(source), _edge(edge){};
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
  inline TriggerSource source() const { return _source; }
  inline void setSource(const TriggerSource& source) { _source = source; }

  inline TriggerType edge() const { return _edge; }
  inline void setEdge(const TriggerType& edge) { _edge = edge; }

  // Members
 private:
  TriggerSource _source;

  TriggerType _edge;
};

}  // namespace uff
