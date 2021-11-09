/*!
 * Copyright Moduleus
 * \file uff_element_geometry.h
 * \brief
 */

#ifndef UFF_ELEMENT_GEOMETRY_H
#define UFF_ELEMENT_GEOMETRY_H

// UFF
#include "uff_object.h"
#include "uff_perimeter.h"

// System
#include <string>

namespace uff
{
    
/**
 * @brief Describes the geometry of an ultrasonic element.
 */
class ElementGeometry : public uff::Object
{
    UFF_TYPE_MACRO(ElementGeometry, uff::Object);

public:

    ElementGeometry() {}

    void printSelf(std::ostream& os, std::string indent) const override;

    uff::Perimeter& perimeter() { return m_perimeter; }
    void setPerimeter(const uff::Perimeter& perimeter) 
    {
        m_perimeter = perimeter;
    }

    bool operator ==(const ElementGeometry& other) const
    {
        return (m_perimeter == other.m_perimeter);
    }

    inline bool operator !=(const ElementGeometry& other) const
    {
        return !(*this == other);
    }
 
private:
    // (Optional) List of positions defining the element perimeter
    uff::Perimeter m_perimeter;
};

} // namespace uff

#endif // UFF_ELEMENT_GEOMETRY_H