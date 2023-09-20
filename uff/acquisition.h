#pragma once

// UFF
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/igroup.h>
#include <uff/probe.h>
#include <uff/receive_setup.h>
#include <uff/super_group.h>
#include <uff/event.h>
#include <uff/transmit_setup.h>
#include <uff/trigger_in.h>
#include <uff/trigger_out.h>

// System
#include <limits>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace uff {

/**
 * @brief UFF class that contains all the information needed to store and later process channel data.
 */
struct Acquisition {
  bool operator==(const Acquisition& other) const {
    bool are_probes_equaled = probes.size() == other.probes.size();
    for (uint32_t i = 0; i < probes.size() && are_probes_equaled; ++i) {
      are_probes_equaled = are_probes_equaled && (*probes[i] == *other.probes[i]);
    }

    bool are_unique_transmit_setups_equaled =
        unique_transmit_setups.size() == other.unique_transmit_setups.size();
    for (uint32_t i = 0; i < unique_transmit_setups.size() && are_unique_transmit_setups_equaled;
         ++i) {
      are_unique_transmit_setups_equaled =
          are_unique_transmit_setups_equaled &&
          (*unique_transmit_setups[i] == *other.unique_transmit_setups[i]);
    }

    bool are_unique_receive_setups_equaled =
        unique_receive_setups.size() == other.unique_receive_setups.size();
    for (uint32_t i = 0; i < unique_receive_setups.size() && are_unique_receive_setups_equaled;
         ++i) {
      are_unique_receive_setups_equaled =
          are_unique_receive_setups_equaled &&
          (*unique_receive_setups[i] == *other.unique_receive_setups[i]);
    }

    bool are_groups_equaled = groups.size() == other.groups.size();
    for (uint32_t i = 0; i < groups.size() && are_groups_equaled; ++i) {
      are_groups_equaled = are_groups_equaled && (*groups[i] == *other.groups[i]);
    }

    bool are_group_data_equaled = group_data.size() == other.group_data.size();
    for (uint32_t i = 0; i < group_data.size() && are_group_data_equaled; ++i) {
      are_group_data_equaled = are_group_data_equaled && (*group_data[i] == *other.group_data[i]);
    }

    bool are_unique_excitations_equaled =
        unique_excitations.size() == other.unique_excitations.size();
    for (uint32_t i = 0; i < unique_excitations.size() && are_unique_excitations_equaled; ++i) {
      are_unique_excitations_equaled = are_unique_excitations_equaled &&
                                       (*unique_excitations[i] == *other.unique_excitations[i]);
    }

    return ((authors == other.authors) && (description == other.description) &&
            (local_time == other.local_time) && (country_code == other.country_code) &&
            (system == other.system) && (sound_speed == other.sound_speed) &&
            (timestamp == other.timestamp) && (trigger_in == other.trigger_in) &&
            (trigger_out == other.trigger_out) && (time_offset == other.time_offset) &&
            (initial_group.lock() == other.initial_group.lock()) && are_probes_equaled &&
            are_unique_transmit_setups_equaled && are_unique_receive_setups_equaled &&
            are_unique_excitations_equaled && are_groups_equaled && are_group_data_equaled);
  }
  inline bool operator!=(const Acquisition& other) const { return !(*this == other); }

  // string with the authors of the data
  std::string authors;

  // string describing the data
  std::string description;

  // string defining the time the dataset was acquired following ISO 8601
  std::string local_time;

  // string defining the country, following ISO 3166-1
  std::string country_code;

  // string defining the system used to acquired the dataset
  std::string system;

  // Reference sound speed for Tx and Rx events [m/s]
  double sound_speed = 0.;

  // Initial group
  std::weak_ptr<IGroup> initial_group;

  // List of all group present in the acquisition
  std::vector<std::shared_ptr<IGroup>> groups;

  // List of all the probes used to transmit/receive sequences in the acquisition
  std::vector<std::shared_ptr<Probe>> probes;

  // List of all the unique receive setup used in the sequences in the acquisition
  std::vector<std::shared_ptr<ReceiveSetup>> unique_receive_setups;

  // List of all the unique transmit setup (wave or beams) used in the sequences in the acquisition
  std::vector<std::shared_ptr<TransmitSetup>> unique_transmit_setups;

  // List of all the unique excitations used the sequences in the acquisition
  std::vector<std::shared_ptr<Excitation>> unique_excitations;

  // Timestamp
  double timestamp = std::numeric_limits<double>::max();

  // List of all data acquired by the running groups in the acquisition
  std::vector<std::shared_ptr<GroupData>> group_data;

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff
