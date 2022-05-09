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
	explicit Position(FloatingType x = 0.f, FloatingType y = 0.f, FloatingType z = 0.f)
	{
		this->setX(x);
		this->setY(y);
		this->setZ(z);
	}

    friend std::ostream& operator<<(std::ostream& os, const uff::Position& r);


	FloatingType x() const { return m_x; }
	void setX(FloatingType x) { m_x = x; }

	FloatingType y() const { return m_y; }
	void setY(FloatingType y) { m_x = y; }

	FloatingType z() const { return m_z; }
	void setZ(FloatingType z) { m_z = z; }

private:
	FloatingType m_x;
	FloatingType m_y;
	FloatingType m_z;
};

} // namespace uff

#endif // UFF_POSTITION_H