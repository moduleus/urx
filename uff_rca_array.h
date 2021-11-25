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

    explicit RcaArray(uint32_t numberElementsX, uint32_t numberElementsy) :
        m_numberElementsX(numberElementsX),
        m_numberElementsY(numberElementsy)
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

    std::optional<double> pitchX() { return m_pitchX; }
    void setPitchX(std::optional<double> pitchX)
    { 
        m_pitchX = pitchX; 
        updateElements();
    }

    std::optional<double> pitchY() { return m_pitchX; }
    void setPitchY(std::optional<double> pitchY)
    { 
        m_pitchY = pitchY;
        updateElements();
    }

    std::optional<double> elementWidthX() { return m_elementWidthX; }
    void setElementWidthX(std::optional<double> elementWidthX) { m_elementWidthX = elementWidthX; }

    std::optional<double> elementWidthY() { return m_elementWidthY; }
    void setElementWidthY(std::optional<double> elementWidthY) { m_elementWidthY = elementWidthY; }

    std::optional<double> elementHeightX() { return m_elementHeightX; }
    void setElementHeightX(std::optional<double> elementHeightX) { m_elementHeightX = elementHeightX; }

    std::optional<double> elementHeightY() { return m_elementHeightY; }
    void setElementHeightY(std::optional<double> elementHeightY) { m_elementHeightY = elementHeightY; }

    virtual std::shared_ptr<uff::Probe> clone() override
    {
        return std::make_shared<uff::RcaArray>(*this);
    }

private:
    // Update elements position
    void updateElements()
    {
        m_elements.resize(m_numberElementsX + m_numberElementsY);

        double pitchX = m_pitchX.has_value() ? m_pitchX.value() : UFF_NAN;
        double xmin = - pitchX * (m_numberElementsX - 1.0) / 2.0;

        for (int i = 0; i < m_numberElementsX; i++)
        {
            uff::Element element;
            if (m_pitchX.has_value())
                element.setX(xmin + i * pitchX);
            else
                element.setX(0.);

            element.setY(0.);
            element.setZ(0.);
            m_elements[i] = element;
        }

        double pitchY = m_pitchY.has_value() ? m_pitchY.value() : UFF_NAN;
        double ymin = - pitchY * (m_numberElementsY - 1.0) / 2.0;
        for (int i = m_numberElementsX; i < m_elements.size(); i++)
        {
            uff::Element element;
            if (m_pitchX.has_value())
                element.setY(ymin + (i- m_numberElementsX) * pitchY);
            else
                element.setY(0.);

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

    // Distance between the acoustic center of adyacent elements along the x-axis [m]
    std::optional<double> m_pitchX = std::nullopt;

    // Distance between the acoustic center of adyacent elements along the y-axis [m]
    std::optional<double> m_pitchY = std::nullopt;

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