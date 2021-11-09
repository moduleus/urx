/*!
 * Copyright Moduleus
 * \file uff_matrix_array.cpp
 * \brief
 */

#include "uff_matrix_array.h"
 
namespace uff 
{

void MatrixArray::printSelf(std::ostream& os, std::string indent) const
{
    uff::Probe::printSelf(os, indent);
    os << indent;
    superclass::printSelf(os, indent + UFF_STD_INDENT);
    os << indent + UFF_STD_INDENT << "NumberElementsX: " << m_numberElementsX << std::endl;
    os << indent + UFF_STD_INDENT << "NumberElementsY: " << m_numberElementsY << std::endl;

    if (m_pitchX.has_value())
        os << indent + UFF_STD_INDENT << "PitchX: " << m_pitchX.value() << std::endl;
    else
        os << indent + UFF_STD_INDENT << "PitchX: " << UNDEFINED << std::endl;

    if (m_pitchY.has_value())
        os << indent + UFF_STD_INDENT << "PitchY: " << m_pitchY.value() << std::endl;
    else
        os << indent + UFF_STD_INDENT << "PitchY: " << UNDEFINED << std::endl;

    if (m_elementWidth.has_value())
        os << indent + UFF_STD_INDENT << "ElementWidth: " << m_elementWidth.value() << std::endl;
    else
        os << indent + UFF_STD_INDENT << "ElementWidth: " << UNDEFINED << std::endl;

    if (m_elementHeight.has_value())
        os << indent + UFF_STD_INDENT << "ElementHeight: " << m_elementHeight.value() << std::endl;
    else
        os << indent + UFF_STD_INDENT << "ElementHeight: " << UNDEFINED << std::endl;
}

} // namespace uff