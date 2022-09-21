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
#include "uff_acquisition.h"

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

        uff::Acquisition& acquisition() { return m_acquisition; }
        const uff::Acquisition& acquisition() const { return m_acquisition; }
        void setAcquisition(const uff::Acquisition& acquisition) { m_acquisition = acquisition; }

        const uff::Version& version() const { return m_version; }
        void setVersion(const uff::Version& version) { m_version = version; }

        /* Convenience access method */

        // Returns the channel geometry of the probe used by the 1st receive setup
        const std::vector<float> getChannelGeometry()
        {
            if (m_acquisition.probes().empty()) { return std::vector<float>(); }
            else { return m_acquisition.probes()[0]->getChannelGeometry(); }
        }

        // Returns the receive delay of the 1st ReceiveSetup
        double getReceiveDelay() const
        {
            if (m_acquisition.uniqueEvents().empty()) { return UFF_NAN; }
            else { return m_acquisition.uniqueEvents()[0]->receiveSetup().timeOffset(); }
        }

        // Returns the type of sampling of the 1st ReceiveSetup
        uff::ReceiveSetup::SAMPLING_TYPE getSamplingType() const
        {
            if (m_acquisition.uniqueEvents().empty()) { return uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF; }
            else { return m_acquisition.uniqueEvents()[0]->receiveSetup().samplingType(); }
        }

        // Return the sampling frequency associated with the 1st receive event [Hz]
        double getSamplingFrequency() const
        {
            if (m_acquisition.uniqueEvents().empty() || !m_acquisition.uniqueEvents()[0]->receiveSetup().samplingFrequency()) { return UFF_NAN; }
            else { return m_acquisition.uniqueEvents()[0]->receiveSetup().samplingFrequency(); }
        }

        // Returns the speed of sound [m/s]
        double getSoundSpeed() const { return m_acquisition.soundSpeed(); }

        // Return the transmit frequency associated with the 1st Wave of the dataset
        double getTransmitFrequency() const
        {
            if (m_acquisition.uniqueWaves().empty() || !m_acquisition.uniqueExcitations()[0]->transmitFrequency().has_value()) { return UFF_NAN; }
            else { return m_acquisition.uniqueExcitations()[0]->transmitFrequency().value(); }
        }

        // Returns true is the 1st probe is of sub-type 'ProbeType'
        // Example: isProbeType<uff::MatrixArray>() == true;
        template <class ProbeType>
        bool isProbeType() const
        {
            if (m_channelData.probes().empty())
            {
                // no probe: not a 'ProbeType' probe
                return false;
            }
            else
            {
                // Try to cast the 1st probe to the user-provided type
                std::shared_ptr<ProbeType> pt = std::dynamic_pointer_cast<ProbeType>(m_channelData.probes()[0]);
                return (pt.get() != nullptr);
            }
        }

        inline bool operator ==(const Dataset& other) const { return ((m_version == other.m_version) && (m_acquisition == other.m_acquisition)); }
        inline bool operator !=(const Dataset& other) const { return !(*this == other); }

    private:
        uff::Version m_version;
        uff::Acquisition m_acquisition;
    };

} // namespace uff

#endif // UFF_DATASET_H
