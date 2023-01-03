/*!
 * Copyright Moduleus
 * \file uff_matrix_array.h
 * \brief
 */

#ifndef UFF_MATRIX_ARRAY_H
#define UFF_MATRIX_ARRAY_H

// UFF
#include "uff_probe.h"

// System
#include <cstdint>
#include <optional>
#include <vector>

namespace uff
{
    
/**
 * @brief The UFF MatrixArray class describes a 2D matrix array of elements
 * Element[0] has the minimum x/y coordinates. Element[1] has the same y-coordinates as
 * element[0] but a higher x-coordinates.
 */
class MatrixArray : public uff::Probe
{
    UFF_TYPE_MACRO(MatrixArray, uff::Object);

public:

    MatrixArray() {}

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

    double pitchY() const { return m_pitchY; }
    void setPitchY(const double& pitchY)
    { 
        m_pitchY = pitchY;
        updateElements();
    }

    std::optional<double> elementWidth() const { return m_elementWidth; }
    void setElementWidth(std::optional<double> elementWidth) { m_elementWidth = elementWidth; }

    std::optional<double> elementHeight() const { return m_elementHeight; }
    void setElementHeight(std::optional<double> elementHeight) { m_elementHeight = elementHeight; }

private:
    // Update elements position
    void updateElements()
    {
        m_elements.resize((size_t)m_numberElementsX * m_numberElementsY);

        double xmin = -m_pitchX * (m_numberElementsX - 1.0) / 2.0;
        double ymin = -m_pitchY * (m_numberElementsY - 1.0) / 2.0;
        for (uint32_t i = 0; i < m_numberElementsY; i++)
        {    
            for (uint32_t j = 0; j < m_numberElementsX; j++)
            {                
                uff::Element element;
                element.setX(xmin + j * m_pitchX);
                element.setY(ymin + i * m_pitchY);
                element.setZ(0.0);
                m_elements[(size_t)j + (size_t)i * m_numberElementsY] = element;
            }
        }
    }

protected:
    // Number of elements in the x-axis
    uint32_t m_numberElementsX = 0;

    // Number of elements in the y-axis
    uint32_t m_numberElementsY = 0;

    // Distance between the acoustic center of adyacent elements along the x-axis [m]
    double m_pitchX = 0;

    // Distance between the acoustic center of adyacent elements along the y-axis [m]
    double m_pitchY = 0;

    // (Optional) Element size in the x-axis [m]
    std::optional<double> m_elementWidth = std::nullopt;

    // (Optional) Element size in the y-axis [m]
    std::optional<double> m_elementHeight = std::nullopt;
};

} // namespace uff

#endif // UFF_MATRIX_ARRAY_H