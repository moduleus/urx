#pragma once

#include <string>
#include <vector>

#include <uff/destination_link.h>
#include <uff/trigger_in.h>
#include <uff/trigger_out.h>

namespace uff {

struct IGroup {
  explicit IGroup(uint32_t repetition_count, std::vector<DestinationLink> destination_links = {},
                  double time_offset = 0., std::string description = "",
                  const std::optional<TriggerIn>& trigger_in = std::nullopt,
                  const std::optional<TriggerOut>& trigger_out = std::nullopt)
      : _description(std::move(description)),
        _repetition_count(repetition_count),
        _destination_links(std::move(destination_links)),
        _trigger_in(trigger_in),
        _trigger_out(trigger_out),
        _time_offset(time_offset) {}
  virtual ~IGroup() = default;

  bool operator==(const IGroup& other) const {
    bool are_destination_links_equaled =
        _destination_links.size() == other._destination_links.size();
    for (uint32_t i = 0; i < _destination_links.size() && are_destination_links_equaled; ++i) {
      are_destination_links_equaled =
          are_destination_links_equaled && (_destination_links[i] == other._destination_links[i]);
    }

    return ((_trigger_in == other._trigger_in) && (_trigger_out == other._trigger_out) &&
            (_time_offset == other._time_offset) && _description == other._description &&
            _repetition_count == other._repetition_count);
  }
  inline bool operator!=(const IGroup& other) const { return !(*this == other); }

  std::string _description;

  // Number of time the sequece will be repeated
  uint32_t _repetition_count = 1u;

  std::vector<DestinationLink> _destination_links;

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> _trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> _trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
