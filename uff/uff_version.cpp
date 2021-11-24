/*!
 * Copyright Moduleus
 * \file uff_version.cpp
 * \brief
 */

#include "uff_version.h"
 
namespace uff 
{
    
void Version::printSelf(std::ostream& os, std::string indent) const
{
    superclass::printSelf(os, indent);
    os << indent << "Major: " << this->m_major << std::endl
        << indent << "Minor: " << this->m_minor << std::endl
        << indent << "Patch: " << this->m_patch << std::endl;
}

} // namespace uff