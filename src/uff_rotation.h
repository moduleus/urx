/*!
 * Copyright Moduleus
 * \file uff_rotation.h
 * \brief
 */

#ifndef UFF_ROTATION_H
#define UFF_ROTATION_H

// UFF
#include "uff_object.h"

namespace uff
{
    
/**
 * @brief The UFF Rotation class contains a rotation in space in spherical coordinates and SI units. 
 * The rotation is specified using Euler angles that are applied in the order ZYX.
 */
class Rotation : public uff::Object
{
    UFF_TYPE_MACRO(Rotation, uff::Object);

public:

    explicit Rotation(double rx = 0.0, double ry = 0.0, double rz = 0.0)
    { 
        setX(rx);
        setY(ry);
        setZ(rz);
    }

    void printSelf(std::ostream& os, std::string indent) const override;
    
    double x() const { return m_rx; }
    void setX(double x) { m_rx = x; }

    double y() const { return m_ry; }
    void setY(double y) { m_ry = y; }

    double z() const { return m_rz; }
    void setZ(double z) { m_rz = z; }
    
    double elevation() const { return m_rx; }
    double azimut() const { return m_ry; }
    double roll() const { return m_rz; }

    bool operator ==(const Rotation& other) const
    {
        return ((m_rx == other.m_rx) &&
            (m_ry == other.m_ry) &&
            (m_rz == other.m_rz));
    }

    inline bool operator !=(const Rotation& other) const
    {
        return !(*this == other);
    }

private:
    // Rotation around the X-axis (elevation) in radians
    double m_rx;

    // Rotation around the Y-axis (azimut) in radians
    double m_ry;

    // Rotation around the Z-axis (roll) in radians
    double m_rz;
};

} // namespace uff

#endif // UFF_ROTATION_H