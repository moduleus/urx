/*!
 * Copyright Moduleus
 * \file uff_position.h
 * \brief
 */

#ifndef UFF_POSTITION_H
#define UFF_POSTITION_H

#include <iostream>

namespace uff
{
    
/**
 * @brief Contains a location in space in Cartesian coordinates and SI units.
 */
class Position
{
public:
	explicit Position(double x = 0.0, double y = 0.0, double z = 0.0)
	{
		this->setX(x);
		this->setY(y);
		this->setZ(z);
	}

    friend std::ostream& operator<<(std::ostream& os, const uff::Position& r);


	double x() const { return m_x; }
	void setX(double x) { m_x = x; }

	double y() const { return m_y; }
	void setY(double y) { m_x = y; }

	double z() const { return m_z; }
	void setZ(double z) { m_z = z; }

private:
	double m_x;
	double m_y;
	double m_z;
};

} // namespace uff

#endif // UFF_POSTITION_H