/*!
 * Copyright Moduleus
 * \file uff_impulse_response.cpp
 * \brief
 */

#include "uff_impulse_response.h"
 
namespace uff 
{
    
void ImpulseResponse::printSelf(std::ostream& os, std::string indent) const
{
    superclass::printSelf(os, indent);
    os << indent << "InitialTime: " << m_initialTime << std::endl;

    if (m_samplingFrequency.has_value())
        os << indent << "SamplingFrequency: " << m_samplingFrequency.value() << std::endl;
    else
        os << indent << "SamplingFrequency: " << UNDEFINED << std::endl;

    os << indent << "Units: " << units() << std::endl;
    os << indent << "Data: " << "TODO" << std::endl;
}

} // namespace uff