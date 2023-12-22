#pragma once

#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <urx/detail/double_nan.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transmit_setup.h>

namespace urx {

struct Acquisition {
  bool operator==(const Acquisition& other) const {
    return authors == other.authors && description == other.description &&
           local_time == other.local_time && country_code == other.country_code &&
           system == other.system && sound_speed == other.sound_speed &&
           timestamp == other.timestamp && probes == other.probes &&
           excitations == other.excitations && waves == other.waves && groups == other.groups &&
           groups_data == other.groups_data;
  }

  bool operator!=(const Acquisition& other) const { return !operator==(other); }

  std::string authors;
  std::string description;
  std::string local_time;
  std::string country_code;
  std::string system;

  DoubleNan sound_speed;
  DoubleNan timestamp;

  std::vector<std::shared_ptr<Probe>> probes;
  std::vector<std::shared_ptr<Excitation>> excitations;
  std::vector<std::shared_ptr<Wave>> waves;
  std::vector<std::shared_ptr<Group>> groups;
  std::vector<std::shared_ptr<GroupData>> groups_data;
};

}  // namespace urx
