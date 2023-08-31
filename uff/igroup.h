#pragma once

#include <string>
#include <vector>

#include <uff/destination_link.h>
#include <uff/trigger_in.h>
#include <uff/trigger_out.h>

namespace uff {

struct IGroup {
  bool operator==(const IGroup& other) const {
    bool are_destination_links_equaled = destination_links.size() == other.destination_links.size();
    for (uint32_t i = 0; i < destination_links.size() && are_destination_links_equaled; ++i) {
      are_destination_links_equaled =
          are_destination_links_equaled && (destination_links[i] == other.destination_links[i]);
    }

    return ((trigger_in == other.trigger_in) && (trigger_out == other.trigger_out) &&
            (time_offset == other.time_offset) && description == other.description &&
            repetition_count == other.repetition_count);
  }
  inline bool operator!=(const IGroup& other) const { return !(*this == other); }

  std::string description;

  // Number of time the sequece will be repeated
  uint32_t repetition_count = 1u;

  std::vector<DestinationLink> destination_links;

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff
