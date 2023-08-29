#pragma once

#include <string>
#include <vector>

#include <uff/time_offset_base.h>
#include <uff/trigger_base.h>

namespace uff {

class IGroup : public TimeOffsetBase, TriggerBase {
 public:
  // CTOR & DTOR
  IGroup() = default;
  explicit IGroup(uint32_t repetition_count,
                  std::vector<std::pair<std::weak_ptr<IGroup>, TriggerIn>> destination_links = {},
                  double time_offset = 0., std::string description = "",
                  const std::optional<TriggerIn>& trigger_in = std::nullopt,
                  const std::optional<TriggerOut>& trigger_out = std::nullopt)
      : TimeOffsetBase(time_offset),
        TriggerBase(trigger_in, trigger_out),
        _description(std::move(description)),
        _repetition_count(repetition_count),
        _destination_links(std::move(destination_links)) {}
  IGroup(const IGroup&) = default;
  IGroup(IGroup&&) noexcept = default;
  ~IGroup() override = default;

  // Operators
  IGroup& operator=(const IGroup& other) noexcept = default;
  IGroup& operator=(IGroup&& other) noexcept = default;
  bool operator==(const IGroup& other) const {
    bool are_destination_links_equaled =
        _destination_links.size() == other._destination_links.size();
    for (uint32_t i = 0; i < _destination_links.size() && are_destination_links_equaled; ++i) {
      are_destination_links_equaled =
          are_destination_links_equaled &&
          (_destination_links[i].first.expired() == other._destination_links[i].first.expired()) &&
          (_destination_links[i].first.expired() ||
           // Compare pointer instead of object to avoid recursive cycling of operator==
           (_destination_links[i].first.lock().get() ==
            other._destination_links[i].first.lock().get())) &&
          (_destination_links[i].second == other._destination_links[i].second);
    }

    return (TimeOffsetBase::operator==(other) && TriggerBase::operator==(other) &&
            _description == other._description && _repetition_count == other._repetition_count);
  }
  inline bool operator!=(const IGroup& other) const { return !(*this == other); }

  // Accessors
  inline std::string description() const { return _description; }
  inline void setDescription(std::string description) { _description = std::move(description); }

  inline double timeOffset() const { return _time_offset; }
  inline void setTimeOffset(double time_offset) { _time_offset = time_offset; }

  inline uint32_t repetitionCount() const { return _repetition_count; }
  inline void setRepetitionCount(uint32_t repetition_count) {
    _repetition_count = repetition_count;
  }

  inline std::vector<std::pair<std::weak_ptr<IGroup>, TriggerIn>>& destinationLinks() {
    return _destination_links;
  }
  inline void setDestinationLinks(
      std::vector<std::pair<std::weak_ptr<IGroup>, TriggerIn>> destination_links) {
    _destination_links = std::move(destination_links);
  }

  // Members
 private:
  std::string _description;

  // Number of time the sequece will be repeated
  uint32_t _repetition_count = 1u;

  std::vector<std::pair<std::weak_ptr<IGroup>, TriggerIn>> _destination_links;
};

}  // namespace uff
