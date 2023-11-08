#pragma once

// UFF
#include <uff/event.h>
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/probe.h>
#include <uff/receive_setup.h>
#include <uff/transmit_setup.h>

// System
#include <limits>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace uff {

/**
 * @brief Main UFF class containing all macro information
 * 
 * The acquisition class is used to store the memory allocation of all main UFF classes.
 * All classes used in the acquisition must be stored in its lists. 
 * If not, the acquisition will be corrupted and retrieved data cannot be processed.
 * 
 * It also reference all data retrieved from the experimentation.
 * 
 * Context information should be described in it, in order to keep track of experience. 
 */
struct Acquisition {
  bool operator==(const Acquisition& other) const {
    bool are_probes_equaled = probes.size() == other.probes.size();
    for (uint32_t i = 0; i < probes.size() && are_probes_equaled; ++i) {
      are_probes_equaled = are_probes_equaled && (*probes[i] == *other.probes[i]);
    }

    bool are_waves_equaled = waves.size() == other.waves.size();
    for (uint32_t i = 0; i < waves.size() && are_waves_equaled; ++i) {
      are_waves_equaled = are_waves_equaled && (*waves[i] == *other.waves[i]);
    }

    bool are_groups_equaled = groups.size() == other.groups.size();
    for (uint32_t i = 0; i < groups.size() && are_groups_equaled; ++i) {
      are_groups_equaled = are_groups_equaled && (*groups[i] == *other.groups[i]);
    }

    bool are_group_data_equaled = group_data.size() == other.group_data.size();
    for (uint32_t i = 0; i < group_data.size() && are_group_data_equaled; ++i) {
      are_group_data_equaled = are_group_data_equaled && (*group_data[i] == *other.group_data[i]);
    }

    bool are_excitations_equaled = excitations.size() == other.excitations.size();
    for (uint32_t i = 0; i < excitations.size() && are_excitations_equaled; ++i) {
      are_excitations_equaled =
          are_excitations_equaled && (*excitations[i] == *other.excitations[i]);
    }

    return ((authors == other.authors) && (description == other.description) &&
            (local_time == other.local_time) && (country_code == other.country_code) &&
            (system == other.system) && (sound_speed == other.sound_speed) &&
            (timestamp == other.timestamp) && are_probes_equaled && are_waves_equaled &&
            are_excitations_equaled && are_groups_equaled && are_group_data_equaled);
  }
  inline bool operator!=(const Acquisition& other) const { return !(*this == other); }

  /// Experimentation authors
  std::string authors = "";

  /// Description of the experimentation
  std::string description = "";

  /// Date of the experimentation acquisition following ISO 8601
  std::string local_time = "";

  /// Contry identification following ISO 3166-1
  std::string country_code = "";

  /// Description of the system used to acquired the dataset
  std::string system = "";

  /// Reference sound speed for Tx and Rx events [m/s]
  /// You must provide the sound speed inside the studied material in order to process the data
  double sound_speed = 0.;

  /// List of all group present in the acquisition
  std::vector<std::shared_ptr<Group>> groups;

  /// List of all the probes used to transmit/receive sequences in the acquisition
  std::vector<std::shared_ptr<Probe>> probes;

  /// List of all the unique waves used in the sequences in the acquisition
  std::vector<std::shared_ptr<Wave>> waves;

  /// List of all the unique excitations used the sequences in the acquisition
  std::vector<std::shared_ptr<Excitation>> excitations;

  /// Timestamp of the launch of the acquisition [s]
  double timestamp = UFF_NAN;

  /// List of all data acquired by the running groups in the acquisition
  std::vector<std::shared_ptr<GroupData>> group_data;
};

}  // namespace uff
