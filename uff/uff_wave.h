/*!
 * Copyright Moduleus
 * \file uff_wave.h
 * \brief
 */

#ifndef UFF_WAVE_H
#define UFF_WAVE_H

// UFF
#include "uff_aperture.h"
#include "uff_excitation.h"
#include "uff_object.h"
#include "uff_transform.h"
#include "uff_types.h"

namespace uff
{
    
/**
 * @brief UFF class to describe the geometry of a transmitted wave or beam
 */
class Wave : public uff::Object
{
    UFF_TYPE_MACRO(Wave, uff::Object);

public:
    Wave() {}

    void printSelf(std::ostream& os, std::string indent) const override;
    
    const uff::Transform& origin() const { return m_origin; }
    void setOrigin(const uff::Transform& origin) { m_origin = origin; }
    
    const uff::WaveType& waveType() const { return m_waveType; }
    void setWaveType(const uff::WaveType& waveType) { m_waveType = waveType; }
    
    const uff::Aperture& aperture() const { return m_aperture; }
    void setAperture(const uff::Aperture& aperture) { m_aperture = aperture; }

    const std::vector<int32_t>& channelMapping() const { return m_channelMapping; }
    void setChannelMapping(const std::vector<int32_t>& channelMapping) { m_channelMapping = channelMapping; }
    
    const std::vector<std::weak_ptr<uff::Excitation>>& channelExcitations() const { return m_channel_excitations; }
    void setChannelExcitations(const std::vector<std::weak_ptr<uff::Excitation>>& excitation) { m_channel_excitations = excitation; }

    const std::vector<float>& channelDcOffset() const { return m_channel_dc_offset; }
    void setChannelDcOffset(const std::vector<float>& channelDcOffset) { m_channel_dc_offset = channelDcOffset; }

    bool operator ==(const Wave& other) const
    {
        bool sameExcitations = m_channel_excitations.size() == other.m_channel_excitations.size();
        for (size_t iExcitation = 0; iExcitation < m_channel_excitations.size(); ++iExcitation)
        {
            sameExcitations &= m_channel_excitations[iExcitation].expired() == other.m_channel_excitations[iExcitation].expired();
            sameExcitations &= m_channel_excitations[iExcitation].expired() || 
                (*m_channel_excitations[iExcitation].lock() == *other.m_channel_excitations[iExcitation].lock());
        }
        return (m_origin == other.m_origin) &&
            (m_waveType == other.m_waveType) &&
            (m_aperture == other.m_aperture) &&
            sameExcitations &&
            m_channel_dc_offset == other.m_channel_dc_offset;
    }

    inline bool operator !=(const Wave& other) const { return !(*this == other); }

private:
    // Geometric origin of the wave.
    uff::Transform m_origin;

    // enumerated type ( [0] -> converging, [1] -> diverging, [2]-> plane, [3]-> cylindrical, [4] -> photoacoustic)
    uff::WaveType m_waveType = uff::WaveType::PLANE_WAVE;

    // Description of the aperture used to produce the wave
    uff::Aperture m_aperture;

    // Channel mapping
    std::vector<int32_t> m_channelMapping;

    // Channel Excitations
    std::vector<std::weak_ptr<uff::Excitation>> m_channel_excitations;

    // Channel DC offset
    std::vector<float> m_channel_dc_offset;
};

} // namespace uff

#endif // UFF_WAVE_H