/*!
 * Copyright Moduleus
 * \file uff_channel_data.cpp
 * \brief
 */

#include "uff_channel_data.h"

namespace uff
{
    
    void ChannelData::printSelf(std::ostream& os, std::string indent) const
    {
        superclass::printSelf(os, indent);
        os << indent << "Authors: " << '"' << authors() << '"' << std::endl;
        os << indent << "Description: " << '"' << description() << '"' << std::endl;
        os << indent << "LocalTime: " << '"' << localTime() << '"' << std::endl;
        os << indent << "CountryCode: " << '"' << countryCode() << '"' << std::endl;
        os << indent << "System: " << '"' << system() << '"' << std::endl;
        os << indent << "SoundSpeed: " << m_soundSpeed << std::endl;

        if (m_repetitionRate.has_value()) { os << indent << "RepetitionRate: " << m_repetitionRate.value() << std::endl; }
        else { os << indent << "RepetitionRate: " << UNDEFINED << std::endl; }

        os << indent << "Probes: (size=" << probes().size() << ")" << std::endl;

        for (int i = 0; i < m_probes.size(); i++)
        {
            const auto& sptr = probes()[i];
            os << indent + UFF_STD_INDENT << '#' << std::setfill('0') << std::setw(8) << i + 1 << ": ";
            (*sptr).printSelf(os, indent + UFF_STD_INDENT + UFF_STD_INDENT);
        }
        
        os << indent << "UniqueWaves: (size=" << m_uniqueWaves.size() << ")" << std::endl;
        for (int i = 0; i < uniqueWaves().size(); i++)
        {
            os << indent + UFF_STD_INDENT << "#" << std::setfill('0') << std::setw(8) << i + 1 << ": ";
            const auto& sptr = uniqueWaves()[i];
            (*sptr).printSelf(os, indent + UFF_STD_INDENT + UFF_STD_INDENT);
        }

        os << indent << "UniqueEvents: (size=" << m_uniqueEvents.size() << ")" << std::endl;
        for (int i = 0; i < uniqueEvents().size(); i++)
        {
            os << indent + UFF_STD_INDENT << "#" << std::setfill('0') << std::setw(8) << i + 1 << ": ";
            const auto& sptr = uniqueEvents()[i];
            (*sptr).printSelf(os, indent + UFF_STD_INDENT + UFF_STD_INDENT);
        }

        os << indent << "Sequence: (size=" << m_sequence.size() << ")" << std::endl;
        for (int i = 0; i < sequence().size(); i++)
        {
            os << indent + UFF_STD_INDENT << "#" << std::setfill('0') << std::setw(8) << i + 1 << ": ";
            const auto& timedEvent = sequence()[i];
            timedEvent.printSelf(os, indent + UFF_STD_INDENT + UFF_STD_INDENT);
        }

        os << indent << "Data: (size=" << m_data.size() << ")\n" << std::endl;
    }

    ChannelData& ChannelData::operator=(const ChannelData& other)
    {
        m_authors = other.m_authors;
        m_description = other.m_description;
        m_localTime = other.m_localTime;
        m_countryCode = other.m_countryCode;
        m_system = other.m_system;
        m_soundSpeed = other.m_soundSpeed;
        m_repetitionRate = other.m_repetitionRate;

        // Probe
        for (auto& probe : other.m_probes)
        {
            std::shared_ptr<uff::Probe> newProbe = probe->clone();
            assert(*newProbe == *probe);
            m_probes.push_back(newProbe);
        }

        // Waves
        for (auto& uniqueWave : other.m_uniqueWaves)
        {
            m_uniqueWaves.push_back(std::make_shared<uff::Wave>());
            *m_uniqueWaves.back() = *uniqueWave;
            assert(*m_uniqueWaves.back() == *uniqueWave);
        }

        // Events
        for (auto& uniqueEvent : other.m_uniqueEvents)
        {
            m_uniqueEvents.push_back(std::make_shared<Event>());
            *m_uniqueEvents.back() = *uniqueEvent;

            // Transmit Probe weak pointer
            std::shared_ptr<uff::Probe> transmitProbe;
            for (size_t iProbe = 0; iProbe < m_probes.size() && !transmitProbe; iProbe++)
            {
                if (*m_probes[iProbe] == *uniqueEvent->transmitSetup().probe().lock())
                {
                    transmitProbe = m_probes[iProbe];
                }
            }
            m_uniqueEvents.back()->transmitSetup().setProbe(transmitProbe);
            
            // Receive Probe weak pointer
            std::shared_ptr<uff::Probe> receiveProbe;
            for (size_t iProbe = 0; iProbe < m_probes.size() && !receiveProbe; iProbe++)
            {
                if (*m_probes[iProbe] == *uniqueEvent->receiveSetup().probe().lock())
                {
                    receiveProbe = m_probes[iProbe];
                }
            }
            m_uniqueEvents.back()->receiveSetup().setProbe(receiveProbe);

            // Wave weak pointer
            std::shared_ptr<uff::Wave> wave;
            for (size_t iWave = 0; iWave < m_uniqueWaves.size() && !wave; iWave++)
            {
                if (*m_uniqueWaves[iWave] == *uniqueEvent->transmitSetup().transmitWave().wave().lock())
                {
                    wave = m_uniqueWaves[iWave];
                }
            }
            m_uniqueEvents.back()->transmitSetup().transmitWave().setWave(wave);

            assert(*m_uniqueEvents.back() == *uniqueEvent);
        }

        // Sequence
        for (auto& timedEvent : other.m_sequence)
        {
            m_sequence.push_back(timedEvent);

            // Event weak pointer
            std::shared_ptr<uff::Event> evenement;
            for (size_t iEvent = 0; iEvent < m_uniqueEvents.size() && !evenement; iEvent++)
            {
                if (*m_uniqueEvents[iEvent] == *timedEvent.evenement().lock())
                {
                    evenement = m_uniqueEvents[iEvent];
                }
            }
            m_sequence.back().setEvent(evenement);

            assert(m_sequence.back() == timedEvent);
        }

        // Data
        m_data = other.m_data;

        m_numberOfFrames = other.m_numberOfFrames;
        m_numberOfEvents = other.m_numberOfEvents;
        m_numberOfChannels = other.m_numberOfChannels;
        m_numberOfSamples = other.m_numberOfSamples;

        assert(*this == other);

        return *this;
    }

} // namespace uff
