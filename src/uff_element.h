/*!
 * Copyright Moduleus
 * \file uff_element.h
 * \brief
 */

#ifndef UFF_ELEMENT_H
#define UFF_ELEMENT_H

 // UFF
#include "uff_element_geometry.h"
#include "uff_impulse_response.h"
#include "uff_object.h"
#include "uff_transform.h"

// System
#include <limits>
#include <optional>

namespace uff
{

/**
 * @brief UFF class to define an ultrasonic element
 */
class Element : public uff::Object
{
    UFF_TYPE_MACRO(Element, uff::Object);

public:

    Element() {}

    void printSelf(std::ostream& os, std::string indent) const override;

    std::optional<float> x() const { return m_x; }
    std::optional<float> y() const { return m_y; }
    std::optional<float> z() const { return m_z; }

    void setX(std::optional<float> x) { m_x = x; }
    void setY(std::optional<float> y) { m_y = y; }
    void setZ(std::optional<float> z) { m_z = z; }
    void setXYZ(std::optional<float> x, std::optional<float> y, std::optional<float> z)
    {
        setX(x);
        setY(y);
        setZ(z);
    }

    bool operator ==(const Element& other) const
    {
        return ((m_x == other.m_x) &&
            (m_y == other.m_y) &&
            (m_z == other.m_z));
    }

    inline bool operator !=(const Element& other) const
    {
        return !(*this == other);
    }

private:
    std::optional<float> m_x = std::nullopt;
    std::optional<float> m_y = std::nullopt;
    std::optional<float> m_z = std::nullopt;

    // Attitude of the element in 3D space
    //uff::Transform m_transform;

    // (Optional) Geometry of the element
    //uff::ElementGeometry m_elementGeometry;

    // (Optional) Electromechanical impulse response
    //uff::ImpulseResponse m_impulseResponse;
};

} // namespace uff

#endif // UFF_ELEMENT_H
