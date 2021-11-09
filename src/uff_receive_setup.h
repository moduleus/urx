/*!
 * Copyright Moduleus
 * \file uff_receive_setup.h
 * \brief
 */

#ifndef UFF_RECEIVE_SETUP_H
#define UFF_RECEIVE_SETUP_H

// UFF
#include "uff_object.h"
#include "uff_probe.h"

// System
#include <iostream>
#include <memory>

namespace uff
{
    
/**
 * @brief The UFF ReceiveSetup class
 */
class ReceiveSetup : public uff::Object
{
    UFF_TYPE_MACRO(ReceiveSetup, uff::Object);

public:

    typedef enum { DIRECT_RF = 0, QUADRATURE_4X_F0 = 1, QUADRATURE_2X_F0 = 2, IQ = 3 } SAMPLING_TYPE;

    ReceiveSetup() {}

    void printSelf(std::ostream& os, std::string indent) const override;
    
    const std::weak_ptr<uff::Probe> probe() const { return m_probe; }
    void setProbe(std::weak_ptr<uff::Probe> probe)
    {
        m_probe = probe;
    }
    
    double timeOffset() const { return m_timeOffset; }
    void setTimeOffset(double timeOffset) 
    {
        m_timeOffset = timeOffset;
    }
    
    std::optional<double> samplingFrequency() const { return m_samplingFrequency; }
    void setSamplingFrequency(std::optional<double> samplingFrequency)
    {
        m_samplingFrequency = samplingFrequency;
    }

    SAMPLING_TYPE samplingType() const { return m_samplingType; }
    void setSamplingType(SAMPLING_TYPE samplingType)
    {
        m_samplingType = samplingType;
    }
    
    std::vector<int> channelMapping() const { return m_channelMapping; }
    void setChannelMapping(const std::vector<int>& channelMapping) 
    {        
        m_channelMapping = channelMapping;
    }

    
    std::vector<float> tgcProfile() const { return m_tgcProfile; }
    void setTgcProfile(const std::vector<float>& tgcProfile)
    {
        m_tgcProfile = tgcProfile;
    }

    std::optional<double> tgcSamplingFrequency() const { return m_tgcSamplingFrequency; }
    void setTgcSamplingFrequency(std::optional<double> tgcSamplingFrequency)
    {
        m_tgcSamplingFrequency = tgcSamplingFrequency;
    }

    std::optional<double> modulationFrequency() const { return m_modulationFrequency; }
    void setModulationFrequency(std::optional<double> modulationFrequency)
    {
        m_modulationFrequency = modulationFrequency;
    }

    bool operator ==(const ReceiveSetup& other) const
    {
        return ( (m_probe.expired() == other.m_probe.expired()) &&
            ( m_probe.expired() || (*(m_probe.lock()) == *(other.m_probe.lock()))) &&
            (m_timeOffset == other.m_timeOffset) &&
            (m_samplingFrequency == other.m_samplingFrequency) &&
            (m_samplingType == other.m_samplingType) &&
            (m_channelMapping == other.m_channelMapping) &&
            (m_tgcProfile == other.m_tgcProfile) &&
            (m_tgcSamplingFrequency == other.m_tgcSamplingFrequency) &&
            (m_modulationFrequency == other.m_modulationFrequency) );
    }

    inline bool operator !=(const ReceiveSetup& other) const
    {
        return !(*this == other);
    }    

private:
    // Probes used for this receive setup
    std::weak_ptr<uff::Probe> m_probe;

    // Time before the first samples [s]
    double m_timeOffset = 0.;

    // Sampling frequency [Hz]
    std::optional<double> m_samplingFrequency = std::nullopt;

    // Type of sampling: 
    SAMPLING_TYPE m_samplingType = DIRECT_RF;

    // Maps the RF line with the corresponding element
    // channel_element_index = m_channelMapping[rf_index] 
    std::vector<int> m_channelMapping;

    // (Optional) Analog TGC profile sampled at tgc_sampling_frequency [dB]
    std::vector<float> m_tgcProfile = { 0.0 };

    //     (Optional) Sampling frequency of the TGC profile [Hz]
    std::optional<double> m_tgcSamplingFrequency = std::nullopt;

    //     (Optional) Modulation frequency used in case of IQ-data [Hz]
    std::optional<double> m_modulationFrequency = std::nullopt;
};

} // namespace uff

#endif // UFF_RECEIVE_SETUP_H
