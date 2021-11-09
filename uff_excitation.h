/*!
 * Copyright Moduleus
 * \file uff_excitation.h
 * \brief
 */

#ifndef UFF_EXCITATION_H
#define UFF_EXCITATION_H

// UFF
#include "uff_object.h"

// System
#include <limits>
#include <optional>

namespace uff
{

/**
 * @brief The UFF Excitation class
 */
class Excitation : public uff::Object
{
    UFF_TYPE_MACRO(Excitation, uff::Object);

public:

    Excitation() {}

    void printSelf(std::ostream& os, std::string indent) const override;

    const std::optional<std::string>& pulseShape() const { return m_pulseShape; }
    void setPulseShape(const std::optional<std::string>& pulseShape) { m_pulseShape = pulseShape; }

    const std::vector<float>& waveform() const { return m_waveform; }
    void setWaveform(const std::vector<float>& waveform) { m_waveform = waveform; }

    std::optional<double> samplingFrequency() const { return m_samplingFrequency; }
    void setSamplingFrequency(std::optional<double> samplingFrequency)
    {
        m_samplingFrequency = samplingFrequency;
    }

    std::optional<double> transmitFrequency() const { return m_transmitFrequency; }
    void setTransmitFrequency(std::optional<double> transmitFrequency)
    {
        m_transmitFrequency = transmitFrequency;
    }

    bool operator ==(const Excitation& other) const
    {
        return ((m_pulseShape == other.m_pulseShape) &&
            (m_transmitFrequency == other.m_transmitFrequency) &&
            (m_waveform == other.m_waveform) &&
            (m_samplingFrequency == other.m_samplingFrequency));
    }

    inline bool operator !=(const Excitation& other) const
    {
        return !(*this == other);
    }

private:
    // String describing the pulse shape (e.g., sinusoidal, square wave, chirp), including necessary parameters
    std::optional<std::string> m_pulseShape = std::nullopt;

    // Transmit frequency [Hz]
    std::optional<double> m_transmitFrequency = std::nullopt;

    // Sampled excitation waveform [normalized units]
    std::vector<float> m_waveform;

    // Sampling frequency of the excitation waveform [Hz]
    std::optional<double> m_samplingFrequency = std::nullopt;
};

} // namespace uff

#endif // UFF_EXCITATION_H
