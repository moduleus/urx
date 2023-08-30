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
    bool are_probes_equaled = _probes.size() == other._probes.size();
    for (uint32_t i = 0; i < _probes.size() && are_probes_equaled; ++i) {
      are_probes_equaled = are_probes_equaled && (*_probes[i] == *other._probes[i]);
    }

    bool are_unique_transmit_setups_equaled =
        _unique_transmit_setups.size() == other._unique_transmit_setups.size();
    for (uint32_t i = 0; i < _unique_transmit_setups.size() && are_unique_transmit_setups_equaled;
         ++i) {
      are_unique_transmit_setups_equaled =
          are_unique_transmit_setups_equaled &&
          (*_unique_transmit_setups[i] == *other._unique_transmit_setups[i]);
    }

    bool are_unique_receive_setups_equaled =
        _unique_receive_setups.size() == other._unique_receive_setups.size();
    for (uint32_t i = 0; i < _unique_receive_setups.size() && are_unique_receive_setups_equaled;
         ++i) {
      are_unique_receive_setups_equaled =
          are_unique_receive_setups_equaled &&
          (*_unique_receive_setups[i] == *other._unique_receive_setups[i]);
    }

    bool are_unique_events_equaled = _unique_events.size() == other._unique_events.size();
    for (uint32_t i = 0; i < _unique_events.size() && are_unique_events_equaled; ++i) {
      are_unique_events_equaled =
          are_unique_events_equaled && (*_unique_events[i] == *other._unique_events[i]);
    }

    bool are_groups_equaled = _groups.size() == other._groups.size();
    for (uint32_t i = 0; i < _groups.size() && are_groups_equaled; ++i) {
      are_groups_equaled = are_groups_equaled && (*_groups[i] == *other._groups[i]);
    }

    bool are_unique_excitations_equaled =
        _unique_excitations.size() == other._unique_excitations.size();
    for (uint32_t i = 0; i < _unique_excitations.size() && are_unique_excitations_equaled; ++i) {
      are_unique_excitations_equaled = are_unique_excitations_equaled &&
                                       (*_unique_excitations[i] == *other._unique_excitations[i]);
    }

    size_t group_data_size = 0u;
    size_t other_group_data_size = 0u;

    bool are_data_type_equaled = _data_type == other._data_type &&
                                 std::is_same_v<decltype(_group_data), decltype(other._group_data)>;
    bool are_group_data_equaled = are_data_type_equaled;
    if (are_data_type_equaled) {
      std::visit(
          [&other_group_data_variant = other._group_data, &group_data_size, &other_group_data_size,
           &are_group_data_equaled](auto&& group_data) {
            using group_data_type =
                std::remove_const_t<std::remove_reference_t<decltype(group_data)>>;
            auto& other_group_data = std::get<group_data_type>(other_group_data_variant);

            group_data_size = group_data.size();
            other_group_data_size = other_group_data.size();

            are_group_data_equaled &= group_data_size == other_group_data_size;
            for (uint32_t i = 0; i < group_data_size && are_group_data_equaled; ++i) {
              are_group_data_equaled =
                  are_group_data_equaled && (*group_data[i] == *other_group_data[i]);
            }
          },
          _group_data);
    }

    return ((_authors == other._authors) && (_description == other._description) &&
            (_local_time == other._local_time) && (_country_code == other._country_code) &&
            (_system == other._system) && (_sound_speed == other._sound_speed) &&
            (_timestamp == other._timestamp) && (_trigger_in == other._trigger_in) &&
            (_trigger_out == other._trigger_out) && (_time_offset == other._time_offset) &&
            (_initial_group.lock() == other._initial_group.lock()) && are_probes_equaled &&
            are_unique_transmit_setups_equaled && are_unique_receive_setups_equaled &&
            are_unique_events_equaled && are_unique_excitations_equaled && are_groups_equaled &&
            are_group_data_equaled);
  }
  inline bool operator!=(const Acquisition& other) const { return !(*this == other); }

  // string with the authors of the data
  std::string _authors;

  // string describing the data
  std::string _description;

  // string defining the time the dataset was acquired following ISO 8601
  std::string _local_time;

  // string defining the country, following ISO 3166-1
  std::string _country_code;

  // string defining the system used to acquired the dataset
  std::string _system;

  // Reference sound speed for Tx and Rx events [m/s]
  double _sound_speed = 0.;

  // Initial group
  std::weak_ptr<IGroup> _initial_group;

  // List of all group present in the acquisition
  std::vector<std::shared_ptr<IGroup>> _groups;

  // List of all the probes used to transmit/receive sequences in the acquisition
  std::vector<std::shared_ptr<Probe>> _probes;

  // List of all the unique transmit/receive events used in the sequences in the acquisition
  std::vector<std::shared_ptr<TimedEvent>> _unique_events;

  // List of all the unique receive setup used in the sequences in the acquisition
  std::vector<std::shared_ptr<ReceiveSetup>> _unique_receive_setups;

  // List of all the unique transmit setup (wave or beams) used in the sequences in the acquisition
  std::vector<std::shared_ptr<TransmitSetup>> _unique_transmit_setups;

  // List of all the unique excitations used the sequences in the acquisition
  std::vector<std::shared_ptr<Excitation>> _unique_excitations;

  // Timestamp
  uint64_t _timestamp = 0u;

  // List of all data acquired by the running groups in the acquisition
  VecGroupDataType _group_data;

  // Data type contained in the group data containers
  DATA_TYPE _data_type = DATA_TYPE::INT16;

  // Trigger in for launching the acquisition element
  std::optional<TriggerIn> _trigger_in = std::nullopt;

  // Trigger out applied by the acquisition element at its launch
  std::optional<TriggerOut> _trigger_out = std::nullopt;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
