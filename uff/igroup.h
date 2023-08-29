#pragma once

#include <string>
#include <vector>

#include <uff/trigger_in.h>
#include <uff/trigger_out.h>

namespace uff {

class IGroup {
 public:
  // CTOR & DTOR
  IGroup() = default;
  explicit IGroup(uint32_t repetition_count,
                  std::vector<std::pair<std::weak_ptr<IGroup>, TriggerIn>> destination_links = {},
                  double time_offset = 0., std::string description = "",
                  const std::optional<TriggerIn>& trigger_in = std::nullopt,
                  const std::optional<TriggerOut>& trigger_out = std::nullopt)
      : _description(std::move(description)),
        _repetition_count(repetition_count),
        _destination_links(std::move(destination_links)),
        _trigger_in(trigger_in),
        _trigger_out(trigger_out),
        _time_offset(time_offset) {}
  IGroup(const IGroup&) = default;
  IGroup(IGroup&&) noexcept = default;
  virtual ~IGroup() = default;

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

    return ((_trigger_in == other._trigger_in) && (_trigger_out == other._trigger_out) &&
            (_time_offset == other._time_offset) && _description == other._description &&
            _repetition_count == other._repetition_count);
  }
  inline bool operator!=(const IGroup& other) const {
    return !(*this == other); }

  // Accessors
  inline std::string description() const {
    return _description; }
  inline void setDescription(std::string description) {
    _description = std::move(description); }

  inline double timeOffset() const {
    return _time_offset; }
  inline void setTimeOffset(double time_offset) {
    _time_offset = time_offset; }

  inline uint32_t repetitionCount() const {
    return _repetition_count; }
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

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> _trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> _trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
