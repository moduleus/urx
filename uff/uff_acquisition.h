/*!
 * Copyright Moduleus
 * \file uff_channel_data.h
 * \brief
 */

#ifndef UFF_CHANNEL_DATA_H
#define UFF_CHANNEL_DATA_H

// UFF
#include "uff_object.h"
#include "uff_igroup.h"
#include "uff_group.h"
#include "uff_super_group.h"
#include "uff_probe.h"
#include "uff_event.h"
#include "uff_wave.h"
#include "uff_group_data.h"
#include "uff_group_link.h"
#include "uff_excitation.h"

// System
#include <optional>
#include <string>
#include <vector>

namespace uff
{

    /**
     * @brief UFF class that contains all the information needed to store and later process channel data.
     */
    class Acquisition : public uff::Object
    {
        UFF_TYPE_MACRO(Acquisition, uff::Object);

    public:
        Acquisition() {}

        void printSelf(std::ostream& os, std::string indent) const override;
        
        // TODO: authors should be a vector of std::string, one author per item
        // TODO: addAuthor(const std::string& author)
        const std::string& authors() const { return m_authors; }
        void setAuthors(const std::string& authors) { m_authors = authors; }
    
        const std::string& description() const { return m_description; }
        void setDescription(const std::string& description) { m_description = description; }

        /**
         * Format: ISO8601
         * Example: 
         *   "2008-09-15T15:53:00"
         *   "2008-09-15T15:53:00+05:00"
         */
        const std::string& localTime() const { return m_localTime; }
        void setLocalTime(const std::string& localTime);
    
        /**
         * Format: ISO3166-1
         * Example:
         *     "FR" for France
         */
        const std::string& countryCode() const { return m_countryCode; }
        void setCountryCode(const std::string& countryCode);

        // 'System' describes the acquisition system used to acquire the data 
        const std::string& system() const { return m_system; }
        void setSystem(const std::string& system) { m_system = system; }
    
        // Speed of sound in m/s 
        double soundSpeed() const { return m_soundSpeed; }
        void setSoundSpeed(double soundSpeed) { m_soundSpeed = soundSpeed; }

        // Time Offset in second
        double timeOffset() const { return m_timeOffset; }
        void setTimeOffset(double timeOffset) { m_timeOffset = timeOffset; }

        // Initial Group
        const std::weak_ptr<IGroup>& initialGroup() const { return m_initialGroup; }
        void setInitialGroup(const std::weak_ptr<IGroup> initialGroup) { m_initialGroup = initialGroup; }

        // List of groups used for this dataset
        const std::vector<std::shared_ptr<uff::IGroup>>& groups() const { return m_groups; }
        void addGroup(std::shared_ptr<uff::IGroup> group) { m_groups.push_back(group); }
        void setGroups(const std::vector<std::shared_ptr<uff::IGroup>>& groups) { m_groups = groups; }

        // List of group links used for this dataset
        const std::vector<std::shared_ptr<uff::GroupLink>>& groupLinks() const { return m_groupLinks; }
        void addGroupLink(std::shared_ptr<uff::GroupLink> groupLink) { m_groupLinks.push_back(groupLink); }
        void setGroupLink(const std::vector<std::shared_ptr<uff::GroupLink>>& groupLinks) { m_groupLinks = groupLinks; }

        // List of group data used for this dataset
        const std::vector<std::shared_ptr<uff::GroupData>>& groupData() const { return m_groupData; }
        void addGroupData(std::shared_ptr<uff::GroupData> groupData) { m_groupData.push_back(groupData); }
        void setGroupData(const std::vector<std::shared_ptr<uff::GroupData>>& groupData) { m_groupData = groupData; }

        // List of probes used for this dataset
        const std::vector<std::shared_ptr<uff::Probe>>& probes() const { return m_probes; }
        void addProbe(std::shared_ptr<uff::Probe> probe) { m_probes.push_back(probe); }
        void setProbes(const std::vector<std::shared_ptr<uff::Probe>>& probes) { m_probes = probes; }

        // List of unique waves used for this dataset
        const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves() const { return m_uniqueWaves; }
        void addUniqueWave(std::shared_ptr<uff::Wave> wave) { m_uniqueWaves.push_back(wave); }
        void setUniqueWaves(const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves) { m_uniqueWaves = uniqueWaves; }

        // List of unique events used for this dataset
        const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents() const { return m_uniqueEvents; }
        void addUniqueEvent(std::shared_ptr<uff::Event> event) { m_uniqueEvents.push_back(event); }
        void setUniqueEvents(const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents) { m_uniqueEvents = uniqueEvents; }

        // List of unique excitations used for this dataset
        const std::vector<std::shared_ptr<Excitation>>& uniqueExcitations() const { return m_uniqueExcitation; }
        void addUniqueExcitation(std::shared_ptr<Excitation> excitation) { m_uniqueExcitation.push_back(excitation); }
        void setUniqueExcitations(const std::vector<std::shared_ptr<Excitation>>& uniqueExcitations) { m_uniqueExcitation = uniqueExcitations; }

        /**
         * Returns true if the input string is ISO8601
         * Example: "2008-09-15T15:53:00"
         */
        static bool isIso8601(const std::string& dateTime);

        /**
         * Returns true if the input string is ISO3166
         * Example: "FR"
         * Note: only checks that it is a 2-letter uppercase
         */
        static bool isIso3166(const std::string& countryCode);

        bool operator ==(const Acquisition& other) const;
        inline bool operator !=(const Acquisition& other) const { return !(*this == other); }

    private:
        // string with the authors of the data
        std::string m_authors;

        // string describing the data
        std::string m_description;

        // string defining the time the dataset was acquired following ISO 8601
        std::string m_localTime;

        // string defining the country, following ISO 3166-1
        std::string m_countryCode;

        // string defining the system used to acquired the dataset
        std::string m_system;

        // Reference sound speed for Tx and Rx events [m/s]
        double m_soundSpeed = 0;

        // Time offset
        double m_timeOffset = 0;

        // Initial group 
        std::weak_ptr<IGroup> m_initialGroup;

        // Group links
        std::vector<std::shared_ptr<GroupLink>> m_groupLinks;

        // Goup list
        std::vector<std::shared_ptr<IGroup>> m_groups;

        // List of the probes used to transmit/recive sequences
        std::vector<std::shared_ptr<uff::Probe>> m_probes;

        // List of the unique transmit/receive events used in sequences
        std::vector<std::shared_ptr<uff::Event>> m_uniqueEvents;

        // List of the unique waves (or beams) used in the sequences
        std::vector<std::shared_ptr<uff::Wave>> m_uniqueWaves;

        // List of the times_events that describe the sequence
        std::vector<std::shared_ptr<Excitation>> m_uniqueExcitation;

        // Timestamp
        uint64_t m_timeStamp = 0;

        // Group Data
        std::vector<std::shared_ptr<GroupData>> m_groupData;
    };

} // namespace uff

#endif // UFF_CHANNEL_DATA_H
