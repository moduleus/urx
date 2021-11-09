/*!
 * Copyright Moduleus
 * \file uff_impulse_response.h
 * \brief
 */

#ifndef UFF_IMPULSE_RESPONSE_H
#define UFF_IMPULSE_RESPONSE_H

// UFF
#include "uff_object.h"

// System
#include <limits>
#include <optional>
#include <string>
#include <vector>

namespace uff
{

/**
 * @brief The UFF ImpulseResponse class specifies a temporal impulse response
 */
class ImpulseResponse : public uff::Object
{
    UFF_TYPE_MACRO(ImpulseResponse, uff::Object);

public:

    ImpulseResponse() {}

    void printSelf(std::ostream& os, std::string indent) const override;

    double initialTime() const { return m_initialTime; }
    void setInitialTime(double initialTime)
    {
        m_initialTime = initialTime;
    }

    std::optional<double> samplingFrequency() const { return m_samplingFrequency; }
    void setSampleFrequency(std::optional<double> samplingFrequency)
    {
        m_samplingFrequency = samplingFrequency;
    }

    const std::vector<double>& data() const { return m_data; }
    void setData(const std::vector<double>& data)
    {
        m_data = data;
    }

    const std::string& units() const { return m_units; }
    void setUnits(const std::string& units)
    {
        m_units = units;
    }

    bool operator ==(const ImpulseResponse& other) const
    {
        return ((m_initialTime == other.m_initialTime) &&
            (m_samplingFrequency == other.m_samplingFrequency) &&
            (m_data == other.m_data) &&
            (m_units == other.m_units));
    }

    inline bool operator !=(const ImpulseResponse& other) const
    {
        return !(*this == other);
    }

private:
    // Time in seconds from the delta excitation until the acquisition of the first sample
    double m_initialTime = 0.;

    // Sampling frequency in Hz
    std::optional<double> m_samplingFrequency = std::nullopt;

    // Collection of samples containing the impulse response
    std::vector<double> m_data;

    // (Optional) Name of the units of the impulse response
    std::string m_units;
};

} // namespace uff

#endif // UFF_IMPULSE_RESPONSE_H
