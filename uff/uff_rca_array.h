/*!
 * Copyright Moduleus
 * \file uff_rca_array.h
 * \brief
 */

#ifndef UFF_RCA_ARRAY_H
#define UFF_RCA_ARRAY_H

// UFF
#include "uff_probe.h"

// System
#include <cstdint>
#include <optional>
#include <vector>

namespace uff
{
    
/**
 * @brief The UFF RCA Array class describes a RCA (Row Column Address) probe
 */
class RcaArray : public uff::Probe
{
    UFF_TYPE_MACRO(RcaArray, uff::Object);

public:

    explicit RcaArray(uint32_t numberElementsX, uint32_t numberElementsy, 
        double pitchX = 0, double pitchY = 0) :
        m_numberElementsX(numberElementsX),
        m_numberElementsY(numberElementsy),
        m_pitchX(pitchX),
        m_pitchY(pitchY)
    {
        updateElements();
    }

    void printSelf(std::ostream& os, std::string indent) const override;
    
    uint32_t numberElementsX() const { return m_numberElementsX; }
    void setNumberElementsX(uint32_t numberElementsX)
    { 
        m_numberElementsX = numberElementsX; 
        updateElements(); 
    }

    uint32_t numberElementsY() const { return m_numberElementsY; }
    void setNumberElementsY(uint32_t numberElementsY)
    { 
        m_numberElementsY = numberElementsY; 
        updateElements(); 
    }

    double pitchX() const { return m_pitchX; }
    void setPitchX(const double& pitchX)
    { 
        m_pitchX = pitchX; 
        updateElements();
    }

    double pitchY()  const { return m_pitchY; }
    void setPitchY(const double& pitchY)
    { 
        m_pitchY = pitchY;
        updateElements();
    }

    std::optional<double> elementWidthX() const { return m_elementWidthX; }
    void setElementWidthX(std::optional<double> elementWidthX) { m_elementWidthX = elementWidthX; }

    std::optional<double> elementWidthY() const { return m_elementWidthY; }
    void setElementWidthY(std::optional<double> elementWidthY) { m_elementWidthY = elementWidthY; }

    std::optional<double> elementHeightX() const { return m_elementHeightX; }
    void setElementHeightX(std::optional<double> elementHeightX) { m_elementHeightX = elementHeightX; }

    std::optional<double> elementHeightY() const { return m_elementHeightY; }
    void setElementHeightY(std::optional<double> elementHeightY) { m_elementHeightY = elementHeightY; }

private:
    // Update elements position
    void updateElements()
    {
        m_elements.resize((size_t)m_numberElementsX + m_numberElementsY);

        double xmin = - m_pitchX * (m_numberElementsX - 1.0) / 2.0;

        for (uint32_t i = 0; i < m_numberElementsX; i++)
        {
            uff::Element element;
            element.setX(xmin + i * m_pitchX);

            element.setY(0.);
            element.setZ(0.);
            m_elements[i] = element;
        }

        double ymin = -m_pitchY * (m_numberElementsY - 1.0) / 2.0;
        for (uint32_t i = m_numberElementsX; i < m_elements.size(); i++)
        {
            uff::Element element;
            element.setY((float)(ymin + ((double)i - (double)m_numberElementsX) * m_pitchY));

            element.setX(0.);
            element.setZ(0.);
            m_elements[i] = element;
        }
    }

protected:
    // Number of elements in the x-axis
    uint32_t m_numberElementsX = 0;

    // Number of elements in the y-axis
    uint32_t m_numberElementsY = 0;

    // Distance between the acoustic center of adjacent elements along the x-axis [m]
    double m_pitchX = 0;

    // Distance between the acoustic center of adjacent elements along the y-axis [m]
    double m_pitchY = 0;

    // (Optional) Element size in the x-axis [m]
    std::optional<double> m_elementWidthX = std::nullopt;

    // (Optional) Element size in the x-axis [m]
    std::optional<double> m_elementWidthY = std::nullopt;

    // (Optional) Element size in the y-axis [m]
    std::optional<double> m_elementHeightX = std::nullopt;

    // (Optional) Element size in the y-axis [m]
    std::optional<double> m_elementHeightY = std::nullopt;
};

} // namespace uff

#endif // UFF_RCA_ARRAY_H