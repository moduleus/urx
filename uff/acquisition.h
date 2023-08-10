#pragma once

// UFF
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/igroup.h>
#include <uff/probe.h>
#include <uff/receive_setup.h>
#include <uff/super_group.h>
#include <uff/time_offset_base.h>
#include <uff/timed_event.h>
#include <uff/transmit_setup.h>
#include <uff/trigger_base.h>

// System
#include <optional>
#include <string>
#include <vector>

namespace uff {

/**
 * @brief UFF class that contains all the information needed to store and later process channel data.
 */
class Acquisition : public TimeOffsetBase, TriggerBase {
 public:
  // CTOR & DTOR
  Acquisition() = default;
  Acquisition(const Acquisition&) = default;
  Acquisition(Acquisition&&) noexcept = default;
  ~Acquisition() override = default;

  // Operators
  Acquisition& operator=(const Acquisition& other) noexcept = default;
  Acquisition& operator=(Acquisition&& other) noexcept = default;
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

    bool are_group_data_equaled = _group_data.size() == other._group_data.size();
    for (uint32_t i = 0; i < _group_data.size() && are_group_data_equaled; ++i) {
      are_group_data_equaled = are_group_data_equaled && (*_group_data[i] == *other._group_data[i]);
    }

    return (TimeOffsetBase::operator==(other) && TriggerBase::operator==(other) &&
            (_authors == other._authors) && (_description == other._description) &&
            (_local_time == other._local_time) && (_country_code == other._country_code) &&
            (_system == other._system) && (_sound_speed == other._sound_speed) &&
            (_timestamp == other._timestamp) &&
            (_initial_group.lock() == other._initial_group.lock()) && are_probes_equaled &&
            are_unique_transmit_setups_equaled && are_unique_receive_setups_equaled &&
            are_unique_events_equaled && are_unique_excitations_equaled && are_groups_equaled &&
            are_group_data_equaled);
  }
  inline bool operator!=(const Acquisition& other) const { return !(*this == other); }

  // Accessors
  inline std::string authors() const { return _authors; }
  inline void setAuthors(std::string authors) { _authors = std::move(authors); }

  inline std::string description() const { return _description; }
  inline void setDescription(std::string description) { _description = std::move(description); }

  /**
     * Should be at format: ISO8601
     * Example: 
     *   "2008-09-15T15:53:00"
     *   "2008-09-15T15:53:00+05:00"
     */
  inline std::string localTime() const { return _local_time; }
  void setLocalTime(const std::string& local_time) { _local_time = local_time; }

  /**
     * Should be at format: ISO3166-1
     * Example:
     *     "FR" for France
     */
  inline std::string countryCode() const { return _country_code; }
  void setCountryCode(const std::string& country_code) { _country_code = country_code; }

  // 'System' describes the acquisition system used to acquire the data
  inline std::string system() const { return _system; }
  inline void setSystem(const std::string& system) { _system = system; }

  // Speed of sound in m/s
  inline MetadataType soundSpeed() const { return _sound_speed; }
  inline void setSoundSpeed(MetadataType sound_speed) { _sound_speed = sound_speed; }

  // Initial Group
  inline const std::weak_ptr<IGroup>& initialGroup() const { return _initial_group; }
  inline void setInitialGroup(const std::weak_ptr<IGroup>& initial_group) {
    _initial_group = initial_group;
  }

  // List of groups used for this dataset
  inline const std::vector<std::shared_ptr<IGroup>>& groups() const { return _groups; }
  inline void addGroup(const std::shared_ptr<IGroup>& group) { _groups.push_back(group); }
  inline void setGroups(const std::vector<std::shared_ptr<IGroup>>& groups) { _groups = groups; }

  // List of group data used for this dataset
  const std::vector<std::shared_ptr<GroupData>>& groupData() const { return _group_data; }
  void addGroupData(const std::shared_ptr<GroupData>& group_data) {
    _group_data.push_back(group_data);
  }
  void setGroupData(const std::vector<std::shared_ptr<GroupData>>& group_data) {
    _group_data = group_data;
  }

  // List of probes used for this dataset
  inline const std::vector<std::shared_ptr<Probe>>& probes() const { return _probes; }
  inline void addProbe(const std::shared_ptr<Probe>& probe) { _probes.push_back(probe); }
  inline void setProbes(const std::vector<std::shared_ptr<Probe>>& probes) { _probes = probes; }

  // List of unique transmit_setups used for this dataset
  inline const std::vector<std::shared_ptr<TransmitSetup>>& uniqueTransmitSetups() const {
    return _unique_transmit_setups;
  }
  inline void addUniqueTransmitSetup(const std::shared_ptr<TransmitSetup>& unique_transmit_setups) {
    _unique_transmit_setups.push_back(unique_transmit_setups);
  }
  inline void setUniqueTransmitSetups(
      const std::vector<std::shared_ptr<TransmitSetup>>& unique_transmit_setups) {
    _unique_transmit_setups = unique_transmit_setups;
  }

  // List of unique receive_setups used for this dataset
  inline const std::vector<std::shared_ptr<ReceiveSetup>>& uniqueReceiveSetups() const {
    return _unique_receive_setups;
  }
  inline void addUniqueReceiveSetup(const std::shared_ptr<ReceiveSetup>& unique_receive_setups) {
    _unique_receive_setups.push_back(unique_receive_setups);
  }
  inline void setUniqueReceiveSetups(
      const std::vector<std::shared_ptr<ReceiveSetup>>& unique_receive_setups) {
    _unique_receive_setups = unique_receive_setups;
  }

  // List of unique events used for this dataset
  inline const std::vector<std::shared_ptr<TimedEvent>>& uniqueEvents() const {
    return _unique_events;
  }
  inline void addUniqueEvent(const std::shared_ptr<TimedEvent>& event) {
    _unique_events.push_back(event);
  }
  inline void setUniqueEvents(const std::vector<std::shared_ptr<TimedEvent>>& unique_events) {
    _unique_events = unique_events;
  }

  // List of unique excitations used for this dataset
  inline const std::vector<std::shared_ptr<Excitation>>& uniqueExcitations() const {
    return _unique_excitations;
  }
  inline void addUniqueExcitation(const std::shared_ptr<Excitation>& excitation) {
    _unique_excitations.push_back(excitation);
  }
  inline void setUniqueExcitations(
      const std::vector<std::shared_ptr<Excitation>>& unique_excitations) {
    _unique_excitations = unique_excitations;
  }

 private:
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
  MetadataType _sound_speed = 0.;

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
  std::vector<std::shared_ptr<GroupData>> _group_data;
};

}  // namespace uff
