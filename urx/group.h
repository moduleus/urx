#pragma once

#include <memory>

#include <urx/enums.h>
#include <urx/event.h>
#include <urx/urx.h>

namespace urx {

namespace detail {
template <class Event>
struct Group {
  bool operator==(const Group& other) const {
    return sampling_type == other.sampling_type && data_type == other.data_type &&
           description == other.description && sequence == other.sequence;
  }

  bool operator!=(const Group& other) const { return !operator==(other); }

  SamplingType sampling_type = SamplingType::UNDEFINED;
  DataType data_type = DataType::UNDEFINED;

  std::string description;

  std::vector<Event> sequence;
};
}  // namespace detail
using Group = detail::Group<Event>;

}  // namespace urx
