/*!
 * Copyright Moduleus
 * \file uff_probe.cpp
 * \brief
 */

#include "uff_probe.h"

namespace uff {

void Probe::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Transform: ";
  this->transform().printSelf(os, indent + UFF_STD_INDENT + UFF_STD_INDENT);

  if (m_focalLength.has_value())
    os << indent << "FocalLength: " << m_focalLength.value() << std::endl;
  else
    os << indent << "FocalLength: " << UNDEFINED << std::endl;

  os << indent << "Elements: (size=" << this->m_elements.size() << ")" << std::endl;
  for (int i = 0; i < 10 /*this->m_elements.size()*/; i++) {
    os << indent + UFF_STD_INDENT << '#' << i + 1 << ": ";
    this->m_elements[i].printSelf(os, "");
  }

  os << indent << "ElementGeometries: (size=" << this->m_elementGeometries.size() << ")"
     << std::endl;
  for (uint32_t i = 0; i < this->m_elementGeometries.size(); i++) {
    os << indent + UFF_STD_INDENT << '#' << i + 1 << ": ";
    this->m_elementGeometries[i].printSelf(os, indent + UFF_STD_INDENT + UFF_STD_INDENT);
  }

  os << indent << "ImpulseResponses: (size=" << this->m_impulseResponses.size() << ")" << std::endl;
  if (this->m_impulseResponses.size())
    for (uint32_t i = 0; i < this->m_impulseResponses.size(); i++) {
      os << indent + UFF_STD_INDENT << '#' << i + 1 << ':' << std::endl;
      this->m_impulseResponses[i].printSelf(os, indent + UFF_STD_INDENT + UFF_STD_INDENT);
    }
}

}  // namespace uff