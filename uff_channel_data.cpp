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

    if (m_soundSpeed.has_value())
        os << indent << "SoundSpeed: " << m_soundSpeed.value() << std::endl;
    else
        os << indent << "SoundSpeed: " << UNDEFINED << std::endl;

    if (m_soundSpeed.has_value())
        os << indent << "RepetitionRate: " << m_repetitionRate.value() << std::endl;
    else
        os << indent << "RepetitionRate: " << UNDEFINED << std::endl;

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

} // namespace uff
