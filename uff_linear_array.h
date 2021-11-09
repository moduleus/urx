/*!
 * Copyright Moduleus
 * \file uff_linear_array.h
 * \brief
 */

#ifndef UFF_LINEAR_ARRAY_H
#define UFF_LINEAR_ARRAY_H

// UFF
#include "uff_probe.h"

// System
#include <cstdint>
#include <vector>

namespace uff
{

/**
 * @brief The UFF LinearArray class describes a linear array (1D)
 */
class LinearArray : public uff::Probe
{
    UFF_TYPE_MACRO(LinearArray, uff::Object);

public:

    explicit LinearArray(uint32_t numberElements) :
        m_numberElements(numberElements)
    {
        updateElements();
    }

    void printSelf(std::ostream& os, std::string indent) const override;

    uint32_t numberElements() const { return m_numberElements; }
    void setNumberElements(uint32_t numberElements)
    { 
        m_numberElements = numberElements; 
        updateElements();
    }
    
    std::optional<double> pitch() { return m_pitch; }
    void setPitch(std::optional<double> pitch)
    { 
        m_pitch = pitch;
        updateElements();
    }

    std::optional<double> elementWidth() { return m_elementWidth; }
    void setElementWidth(std::optional<double> elementWidth) { m_elementWidth = elementWidth; }

    std::optional<double> elementHeight() { return m_elementHeight; }
    void setElementHeight(std::optional<double> elementHeight) { m_elementHeight = elementHeight; }

private:
    // Update elements position
    void updateElements()
    {
        m_elements.resize(m_numberElements);
        for (uint32_t i = 0; i < m_numberElements; ++i)
        {
            // element position
            uff::Element element;
            if (m_pitch.has_value())
            {
                double xmin = - m_pitch.value() * (m_numberElements - 1.0) / 2.0;
                element.setX(xmin + i * m_pitch.value());
            }
            else
                element.setX(0.);
            element.setY(0.);
            element.setZ(0.);
            m_elements[i] = element;
        }
    }
    
protected:
    // Number of elements in the array
    uint32_t m_numberElements = 0;

    // Distance between the acoustic ceneter of adyacent elements [m]
    std::optional<double> m_pitch = std::nullopt;

    // (Optional) Element size in the x-axis [m]
    std::optional<double> m_elementWidth = std::nullopt;

    // (Optional) Element size in the y-axis [m]
    std::optional<double> m_elementHeight = std::nullopt;
};

} // namespace uff

#endif // UFF_LINEAR_ARRAY_H