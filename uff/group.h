#pragma once

#include <uff/destination_link.h>
#include <uff/igroup.h>
#include <uff/sequence.h>
#include <uff/uff.h>

namespace uff {

class Group : public IGroup {
 public:
  Group() = delete;
  Group(Sequence sequence, double repetition_rate, uint32_t repetition_count,
        std::vector<DestinationLink> destination_links = {}, double time_offset = 0.)
      : IGroup(repetition_count, std::move(destination_links), time_offset),
        _sequence(std::move(sequence)),
        _repetition_rate(repetition_rate) {}
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
  inline double repetitionRate() const { return _repetition_rate; }
  inline void setRepetitionRate(double repetition_rate) { _repetition_rate = repetition_rate; }

  inline const Sequence& sequence() const { return _sequence; }
  inline void setSequence(const Sequence& sequence) { _sequence = sequence; }

  // Members
 private:
  Sequence _sequence;

  double _repetition_rate = 0.;
};

}  // namespace uff
