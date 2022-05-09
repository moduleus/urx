/*!
 * Copyright Moduleus
 * \file uff_perimeter.h
 * \brief
 */

#ifndef UFF_PERIMETER_H
#define UFF_PERIMETER_H

// UFF
#include "uff_object.h"

// System
#include <vector>
#include <utility>
#include <string>

namespace uff
{
    
/**
 * @brief The UFF Perimeter class
 * TODO: replace std<<>> by uff::Position
 * TODO: << operator for adding points to perimeter
 */
class Perimeter : public uff::Object
{
    UFF_TYPE_MACRO(Perimeter, uff::Object);

public:

    Perimeter() {}

    void printSelf(std::ostream& os, std::string indent) const override;
    
    std::vector< std::pair<FloatingType, FloatingType> >& points() { return m_points; }
    void setPoints(const std::vector< std::pair<FloatingType, FloatingType> >& points) 
    {
        m_points = points;
    }

    bool operator ==(const Perimeter& other) const
    {
        return (m_points == other.m_points);
    }

    inline bool operator !=(const Perimeter& other) const
    {
        return !(*this == other);
    }
 
private:
    // Vector of pair of points (x, y). It is assumed that perimeter is always in the X-Y plane.
    std::vector< std::pair<FloatingType, FloatingType> > m_points;
};

} // namespace uff

#endif // UFF_PERIMETER_H