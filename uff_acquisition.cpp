/*!
 * Copyright Moduleus
 * \file uff_acquisition.cpp
 * \brief
 */

#include "uff_acquisition.h"

// System
#include <regex>


namespace uff
{
    
    void Acquisition::printSelf(std::ostream& os, std::string indent) const
    {
        superclass::printSelf(os, indent);
        os << indent << "Authors: " << '"' << authors() << '"' << std::endl;
        os << indent << "Description: " << '"' << description() << '"' << std::endl;
        os << indent << "LocalTime: " << '"' << localTime() << '"' << std::endl;
        os << indent << "CountryCode: " << '"' << countryCode() << '"' << std::endl;
        os << indent << "System: " << '"' << system() << '"' << std::endl;
        os << indent << "SoundSpeed: " << m_soundSpeed << std::endl;

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
    }

    void Acquisition::setLocalTime(const std::string& localTime)
    {
        // validate
        if (isIso8601(localTime)) { m_localTime = localTime; }
        else { std::cerr << '"' << localTime << "\" is not ISO8601 format (YYYY-MM-DDThh:mm:ss)\n"; }
    }

    void Acquisition::setCountryCode(const std::string& countryCode)
    {
        // validate
        if (isIso3166(countryCode)) { m_countryCode = countryCode; }
        else { std::cerr << '"' << countryCode << "\" is not ISO3166 (XX)\n"; }
    }

    bool Acquisition::isIso8601(const std::string& dateTime)
    {
        const std::string r = R"(^([\+-]?\d{4}(?!\d{2}\b))((-?)((0[1-9]|1[0-2])(\3([12]\d|0[1-9]|3[01]))?|W([0-4]\d|5[0-2])(-?[1-7])?|(00[1-9]|0[1-9]\d|[12]\d{2}|3([0-5]\d|6[1-6])))([T\s]((([01]\d|2[0-3])((:?)[0-5]\d)?|24\:?00)([\.,]\d+(?!:))?)?(\17[0-5]\d([\.,]\d+)?)?([zZ]|([\+-])([01]\d|2[0-3]):?([0-5]\d)?)?)?)?$)";
        //const std::string r = R"(^(?:[1-9]\d{3}-(?:(?:0[1-9]|1[0-2])-(?:0[1-9]|1\d|2[0-8])|(?:0[13-9]|1[0-2])-(?:29|30)|(?:0[13578]|1[02])-31)|(?:[1-9]\d(?:0[48]|[2468][048]|[13579][26])|(?:[2468][048]|[13579][26])00)-02-29)T(?:[01]\d|2[0-3]):[0-5]\d:[0-5]\d(?:\.\d{1,9})?(?:Z|[+-][01]\d:[0-5]\d)$)";
        //const std::string r = R"(^(-?(?:[1-9][0-9]*)?[0-9]{4})-(1[0-2]|0[1-9])-(3[01]|0[1-9]|[12][0-9])T(2[0-3]|[01][0-9]):([0-5][0-9]):([0-5][0-9])(.[0-9]+)?(Z)?$)";
        return std::regex_match(dateTime, std::regex(r));
    }

    bool Acquisition::isIso3166(const std::string& countryCode)
    {
        const std::string r = R"(^[A-Z][A-Z]$)";
        return std::regex_match(countryCode, std::regex(r));
    }

    bool Acquisition::operator==(const Acquisition& other) const
    {
        bool areProbesEqualed = true;
        for (uint32_t i = 0; i < m_probes.size() && areProbesEqualed; ++i)
        {
            areProbesEqualed = areProbesEqualed && (*m_probes[i] == *other.m_probes[i]);
        }

        bool areUniqueWavesEqualed = true;
        for (uint32_t i = 0; i < m_uniqueWaves.size() && areUniqueWavesEqualed; ++i)
        {
            areUniqueWavesEqualed = areUniqueWavesEqualed && (*m_uniqueWaves[i] == *other.m_uniqueWaves[i]);
        }

        bool areUniqueEventsEqualed = true;
        for (uint32_t i = 0; i < m_uniqueEvents.size() && areUniqueEventsEqualed; ++i)
        {
            areUniqueEventsEqualed = areUniqueEventsEqualed && (*m_uniqueEvents[i] == *other.m_uniqueEvents[i]);
        }

        return (    (m_authors == other.m_authors) &&
                    (m_description == other.m_description) &&
                    (m_localTime == other.m_localTime) &&
                    (m_countryCode == other.m_countryCode) &&
                    (m_system == other.m_system) &&
                    (m_soundSpeed == other.m_soundSpeed) &&
                    areProbesEqualed &&
                    areUniqueWavesEqualed &&
                    areUniqueEventsEqualed
            );
    }

} // namespace uff
