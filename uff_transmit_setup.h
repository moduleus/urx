/*!
 * Copyright Moduleus
 * \file uff_transmit_setup.h
 * \brief
 */

#ifndef UFF_TRANSMIT_SETUP_H
#define UFF_TRANSMIT_SETUP_H

// UFF
#include "uff_object.h"
#include "uff_probe.h"
#include "uff_wave.h"

namespace uff
{
    
/**
 * @brief The UFF TransmitSetup class
 */
class TransmitSetup : public uff::Object
{
    UFF_TYPE_MACRO(TransmitSetup, uff::Object);

public:

    TransmitSetup() {}

    void printSelf(std::ostream& os, std::string indent) const override;
    
    std::weak_ptr<uff::Probe> probe() const { return m_probe; }
    void setProbe(std::weak_ptr<uff::Probe> probe) { m_probe = probe; }
    
    std::weak_ptr<uff::Wave> wave() const { return m_wave; }
    void setWave(const std::weak_ptr<uff::Wave>& wave) { m_wave = wave; }
    
    double timeOffset() const { return m_timeOffset; }
    void setTimeOffset(double timeOffset) { m_timeOffset = timeOffset; }

    bool operator ==(const TransmitSetup& other) const
    {
        return ( (m_probe.expired() == other.m_probe.expired()) &&
                ( m_probe.expired() || (*(m_probe.lock()) == *(other.m_probe.lock())) ) &&
            ((m_wave.expired() == other.m_wave.expired()) &&
                (m_wave.expired() || (*(m_wave.lock()) == *(other.m_wave.lock()))) &&
            (m_timeOffset == other.m_timeOffset)) );
    }

    inline bool operator !=(const TransmitSetup& other) const { return !(*this == other); }
    
private:
    // Reference to the probe use in transmission
    std::weak_ptr<uff::Probe> m_probe;

    // Wave
    std::weak_ptr<uff::Wave> m_wave;

    // Time offset
    double m_timeOffset = 0;
};

} // namespace uff

#endif // UFF_TRANSMIT_SETUP_H