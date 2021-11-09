/*!
 * Copyright Moduleus
 * \file uff_translation.cpp
 * \brief
 */

#include "uff_translation.h"
 
namespace uff 
{

void Translation::printSelf(std::ostream& os, std::string indent) const
{
    superclass::printSelf(os, indent);
    os << indent 
        << "X: " << this->x() << ", "
        << "Y: " << this->y() << ", "
        << "Z: " << this->z() << std::endl;
}

} // namespace uff
