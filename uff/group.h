#pragma once

#include <uff/igroup.h>
#include <uff/sequence.h>
#include <uff/uff.h>

namespace uff {

class Group : public IGroup {
 public:
  Group() = delete;
  Group(const Sequence& sequence, MetadataType repetition_rate)
      : _sequence(sequence), _repetition_rate(repetition_rate) {}
  Group(const Group&) = default;
  Group(Group&&) noexcept = default;
  ~Group() override = default;

  // Operators
  Group& operator=(const Group& other) noexcept = default;
  Group& operator=(Group&& other) noexcept = default;
  bool operator==(const Group& other) const {
    return (IGroup::operator==(other) && _repetition_rate == other._repetition_rate &&
            _sequence == other._sequence);
  }
  inline bool operator!=(const Group& other) const { return !(*this == other); }

  // Accessors
  inline MetadataType repetitionRate() const { return _repetition_rate; }
  inline void setRepetitionRate(MetadataType repetition_rate) {
    _repetition_rate = repetition_rate;
  }

  inline const Sequence& sequence() const { return _sequence; }
  inline void setSequence(const Sequence& sequence) { _sequence = sequence; }

  // Members
 private:
  Sequence _sequence;

  MetadataType _repetition_rate = 0.;
};

}  // namespace uff
