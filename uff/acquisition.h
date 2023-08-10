#pragma once

// UFF
#include <uff/excitation.h>
#include <uff/group.h>
#include <uff/group_data.h>
#include <uff/group_link.h>
#include <uff/igroup.h>
#include <uff/probe.h>
#include <uff/super_group.h>
#include <uff/time_offset_base.h>
#include <uff/timed_event.h>
#include <uff/trigger_base.h>
#include <uff/wave.h>

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
    bool are_probes_equaled = true;
    for (uint32_t i = 0; i < _probes.size() && are_probes_equaled; ++i) {
      are_probes_equaled = are_probes_equaled && (*_probes[i] == *other._probes[i]);
    }

    bool are_unique_waves_equaled = true;
    for (uint32_t i = 0; i < _unique_waves.size() && are_unique_waves_equaled; ++i) {
      are_unique_waves_equaled =
          are_unique_waves_equaled && (*_unique_waves[i] == *other._unique_waves[i]);
    }

    bool are_unique_events_equaled = true;
    for (uint32_t i = 0; i < _unique_events.size() && are_unique_events_equaled; ++i) {
      are_unique_events_equaled =
          are_unique_events_equaled && (*_unique_events[i] == *other._unique_events[i]);
    }

    bool are_groups_equaled = true;
    for (uint32_t i = 0; i < _groups.size() && are_groups_equaled; ++i) {
      are_groups_equaled = are_groups_equaled && (*_groups[i] == *other._groups[i]);
    }

    bool are_group_links_equaled = true;
    for (uint32_t i = 0; i < _group_links.size() && are_group_links_equaled; ++i) {
      are_group_links_equaled =
          are_group_links_equaled && (*_group_links[i] == *other._group_links[i]);
    }

    bool are_unique_excitations_equaled = true;
    for (uint32_t i = 0; i < _unique_excitation.size() && are_unique_excitations_equaled; ++i) {
      are_unique_excitations_equaled = are_unique_excitations_equaled &&
                                       (*_unique_excitation[i] == *other._unique_excitation[i]);
    }

    bool are_group_data_equaled = true;
    for (uint32_t i = 0; i < _group_data.size() && are_group_data_equaled; ++i) {
      are_group_data_equaled = are_group_data_equaled && (*_group_data[i] == *other._group_data[i]);
    }

    return (TimeOffsetBase::operator==(other) && TriggerBase::operator==(other) &&
            (_authors == other._authors) && (_description == other._description) &&
            (_local_time == other._local_time) && (_country_code == other._country_code) &&
            (_system == other._system) && (_sound_speed == other._sound_speed) &&
            (_timestamp == other._timestamp) &&
            (_initial_group.lock() == other._initial_group.lock()) && are_probes_equaled &&
            are_unique_waves_equaled && are_unique_events_equaled &&
            are_unique_excitations_equaled && are_group_links_equaled && are_groups_equaled &&
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

  // List of group links used for this dataset
  const std::vector<std::shared_ptr<GroupLink>>& groupLinks() const { return _group_links; }
  void addGroupLink(const std::shared_ptr<GroupLink>& group_link) {
    _group_links.push_back(group_link);
  }
  void setGroupLink(const std::vector<std::shared_ptr<GroupLink>>& group_links) {
    _group_links = group_links;
  }

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

  // List of unique waves used for this dataset
  inline const std::vector<std::shared_ptr<Wave>>& uniqueWaves() const { return _unique_waves; }
  inline void addUniqueWave(const std::shared_ptr<Wave>& wave) { _unique_waves.push_back(wave); }
  inline void setUniqueWaves(const std::vector<std::shared_ptr<Wave>>& unique_waves) {
    _unique_waves = unique_waves;
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
    return _unique_excitation;
  }
  inline void addUniqueExcitation(const std::shared_ptr<Excitation>& excitation) {
    _unique_excitation.push_back(excitation);
  }
  inline void setUniqueExcitations(
      const std::vector<std::shared_ptr<Excitation>>& unique_excitations) {
    _unique_excitation = unique_excitations;
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

  // List of all group link present in the acquisition
  std::vector<std::shared_ptr<GroupLink>> _group_links;

  // List of all group present in the acquisition
  std::vector<std::shared_ptr<IGroup>> _groups;

  // List of all the probes used to transmit/receive sequences in the acquisition
  std::vector<std::shared_ptr<Probe>> _probes;

  // List of all the unique transmit/receive events used in the sequences in the acquisition
  std::vector<std::shared_ptr<TimedEvent>> _unique_events;

  // List of all the unique waves (or beams) used in the sequences in the acquisition
  std::vector<std::shared_ptr<Wave>> _unique_waves;

  // List of all the unique excitations used the sequences in the acquisition
  std::vector<std::shared_ptr<Excitation>> _unique_excitation;

  // Timestamp
  uint64_t _timestamp = 0u;

  // List of all data acquired by the running groups in the acquisition
  std::vector<std::shared_ptr<GroupData>> _group_data;
};

}  // namespace uff
