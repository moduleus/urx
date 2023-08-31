#pragma once

// UFF
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/igroup.h>
#include <uff/probe.h>
#include <uff/receive_setup.h>
#include <uff/super_group.h>
#include <uff/timed_event.h>
#include <uff/transmit_setup.h>
#include <uff/trigger_in.h>
#include <uff/trigger_out.h>

// System
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace uff {

/**
 * @brief UFF class that contains all the information needed to store and later process channel data.
 */
struct Acquisition {
  using VecGroupDataType = std::variant<std::vector<std::shared_ptr<GroupData<int16_t>>>,
                                        std::vector<std::shared_ptr<GroupData<float>>>,
                                        std::vector<std::shared_ptr<GroupData<double>>>>;

  enum class DATA_TYPE { INT16 = 0, FLOAT = 1, DOUBLE = 2 };

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

    bool are_unique_events_equaled = unique_events.size() == other.unique_events.size();
    for (uint32_t i = 0; i < unique_events.size() && are_unique_events_equaled; ++i) {
      are_unique_events_equaled =
          are_unique_events_equaled && (*unique_events[i] == *other.unique_events[i]);
    }

    bool are_groups_equaled = groups.size() == other.groups.size();
    for (uint32_t i = 0; i < groups.size() && are_groups_equaled; ++i) {
      are_groups_equaled = are_groups_equaled && (*groups[i] == *other.groups[i]);
    }

    bool are_unique_excitations_equaled =
        unique_excitations.size() == other.unique_excitations.size();
    for (uint32_t i = 0; i < unique_excitations.size() && are_unique_excitations_equaled; ++i) {
      are_unique_excitations_equaled = are_unique_excitations_equaled &&
                                       (*unique_excitations[i] == *other.unique_excitations[i]);
    }

    size_t group_data_size = 0u;
    size_t other_group_data_size = 0u;

    bool are_data_type_equaled = data_type == other.data_type &&
                                 std::is_same_v<decltype(group_data), decltype(other.group_data)>;
    bool are_group_data_equaled = are_data_type_equaled;
    if (are_data_type_equaled) {
      std::visit(
          [&other_group_data_variant = other.group_data, &group_data_size, &other_group_data_size,
           &are_group_data_equaled](auto&& this_group_data) {
            using group_data_type =
                std::remove_const_t<std::remove_reference_t<decltype(this_group_data)>>;
            auto& other_group_data = std::get<group_data_type>(other_group_data_variant);

            group_data_size = this_group_data.size();
            other_group_data_size = other_group_data.size();

            are_group_data_equaled &= group_data_size == other_group_data_size;
            for (uint32_t i = 0; i < group_data_size && are_group_data_equaled; ++i) {
              are_group_data_equaled =
                  are_group_data_equaled && (*this_group_data[i] == *other_group_data[i]);
            }
          },
          group_data);
    }

    return ((authors == other.authors) && (description == other.description) &&
            (local_time == other.local_time) && (country_code == other.country_code) &&
            (system == other.system) && (sound_speed == other.sound_speed) &&
            (timestamp == other.timestamp) && (trigger_in == other.trigger_in) &&
            (trigger_out == other.trigger_out) && (time_offset == other.time_offset) &&
            (initial_group.lock() == other.initial_group.lock()) && are_probes_equaled &&
            are_unique_transmit_setups_equaled && are_unique_receive_setups_equaled &&
            are_unique_events_equaled && are_unique_excitations_equaled && are_groups_equaled &&
            are_group_data_equaled);
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

  // List of all the unique transmit/receive events used in the sequences in the acquisition
  std::vector<std::shared_ptr<TimedEvent>> unique_events;

  // List of all the unique receive setup used in the sequences in the acquisition
  std::vector<std::shared_ptr<ReceiveSetup>> unique_receive_setups;

  // List of all the unique transmit setup (wave or beams) used in the sequences in the acquisition
  std::vector<std::shared_ptr<TransmitSetup>> unique_transmit_setups;

  // List of all the unique excitations used the sequences in the acquisition
  std::vector<std::shared_ptr<Excitation>> unique_excitations;

  // Timestamp
  uint64_t timestamp = 0u;

  // List of all data acquired by the running groups in the acquisition
  VecGroupDataType group_data;

  // Data type contained in the group data containers
  DATA_TYPE data_type = DATA_TYPE::INT16;

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff
