#pragma once

#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <urx/v0_3/event.h>
#include <urx/v0_3/group_data.h>
#include <urx/v0_3/group_link.h>
#include <urx/v0_3/igroup.h>
#include <urx/v0_3/probe.h>
#include <urx/v0_3/trigger_in.h>
#include <urx/v0_3/trigger_out.h>

namespace urx::v0_3 {

struct Acquisition {
  std::string authors;
  std::string description;
  std::string local_time;
  std::string country_code;
  std::string system;

  double sound_speed;

  std::vector<std::shared_ptr<TriggerIn>> trigger_in;
  std::vector<std::shared_ptr<TriggerOut>> trigger_out;
  std::shared_ptr<IGroup> initial_group;

  std::vector<std::shared_ptr<GroupLink>> group_link;
  std::vector<std::shared_ptr<IGroup>> groups;
  std::vector<std::shared_ptr<Probe>> probes;
  std::vector<std::shared_ptr<Event>> unique_events;
  std::vector<std::shared_ptr<Wave>> waves;
  std::vector<std::shared_ptr<Excitation>> excitation;

  uint64_t timestamp;
  std::vector<std::shared_ptr<GroupData>> group_data;
};

}  // namespace urx::v0_3
