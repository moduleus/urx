/*!
 * Copyright Moduleus
 * \file uff_dataset.h
 * \brief
 */

#ifndef UFF_DATASET_H
#define UFF_DATASET_H

// UFF
#include "uff_object.h"
#include "uff_version.h"
#include "uff_channel_data.h"

// System
#include <cmath>

namespace uff
{

/**
 * @brief The UFF Dataset class
 */
class Dataset : public uff::Object
{
    UFF_TYPE_MACRO(Dataset, uff::Object);

public:

    Dataset() {}

    void printSelf(std::ostream& os, std::string indent) const override;

    uff::ChannelData& channelData() { return m_channelData; }
    void setChannelData(const uff::ChannelData& channelData) { m_channelData = channelData; }

    const uff::Version& version() const { return m_version; }
    void setVersion(const uff::Version& version) { m_version = version; }

    // ___________________ Convenience access method ___________________________________

    // Returns the channel geometry of the probe used by the 1st receive setup
    const std::vector<FloatingType> getChannelGeometry()
    {
        if (m_channelData.probes().empty()) { return std::vector<FloatingType>(); }
        else { return m_channelData.probes()[0]->getChannelGeometry(); }
    }

    // Returns the receive delay of the 1st ReceiveSetup
    FloatingType getReceiveDelay() const
    {
        if (m_channelData.uniqueEvents().empty()) { return UFF_NAN; }
        else
        {  return m_channelData.uniqueEvents()[0]->receiveSetup().timeOffset();
        }
    }

    // Returns the type of sampling of the 1st ReceiveSetup
    uff::ReceiveSetup::SAMPLING_TYPE getSamplingType() const
    {
        if (m_channelData.uniqueEvents().empty()) { return uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF; }
        else { return m_channelData.uniqueEvents()[0]->receiveSetup().samplingType(); }
    }

    // Return the sampling frequency associated with the 1st receive event [Hz]
    FloatingType getSamplingFrequency() const
    {
        if (m_channelData.uniqueEvents().empty() || 
            !m_channelData.uniqueEvents()[0]->receiveSetup().samplingFrequency().has_value()) { return UFF_NAN; }
        else { return m_channelData.uniqueEvents()[0]->receiveSetup().samplingFrequency().value(); }
    }

    // Returns the speed of sound [m/s]
    FloatingType getSoundSpeed() const { return m_channelData.soundSpeed(); }

    // Return the transmit frequency associated with the 1st Wave of the dataset
    FloatingType getTransmitFrequency() const
    {
        if (m_channelData.uniqueWaves().empty() || 
            !m_channelData.uniqueWaves()[0]->excitation().transmitFrequency().has_value())
        {
            return UFF_NAN;
        }
        else
        {
            return m_channelData.uniqueWaves()[0]->excitation().transmitFrequency().value();
        }
    }

    // Returns true is the 1st probe is of sub-type 'ProbeType'
    // Example: isProbeType<uff::MatrixArray>() == true;
    template <class ProbeType>
    bool isProbeType() const
    {
        if (m_channelData.probes().empty()) { return false; }
        else
        {
            // Try to cast the 1st probe to the user-provided type
            std::shared_ptr<ProbeType> pt = std::dynamic_pointer_cast<ProbeType>(m_channelData.probes()[0]);
            return (pt.get() != nullptr);
        }
    }

    inline bool operator ==(const Dataset& other) const
    {
        return ((m_version == other.m_version) && 
            (m_channelData == other.m_channelData));
    }

    inline bool operator !=(const Dataset& other) const { return !(*this == other); }

private:
    uff::Version m_version;
    uff::ChannelData m_channelData;
};

} // namespace uff

#endif // UFF_DATASET_H
