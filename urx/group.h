#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/detail/compare.h>  // IWYU pragma: keep
#include <urx/enums.h>
#include <urx/event.h>

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
