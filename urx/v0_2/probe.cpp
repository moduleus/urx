// Copyright Moduleus

#include <cstdint>
#include <ios>
#include <ostream>
#include <utility>

#include <urx/v0_2/element.h>
#include <urx/v0_2/element_geometry.h>
#include <urx/v0_2/impulse_response.h>
#include <urx/v0_2/object.h>
#include <urx/v0_2/probe.h>
#include <urx/v0_2/transform.h>

namespace urx::v0_2 {

void Probe::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Transform: ";
  this->transform().printSelf(os, indent + URX_STD_INDENT + URX_STD_INDENT);

  if (m_focalLength.has_value())
    os << indent << "FocalLength: " << m_focalLength.value() << '\n';
  else
    os << indent << "FocalLength: " << UNDEFINED << '\n';

  os << indent << "Elements: (size=" << this->m_elements.size() << ")" << '\n';
  for (int i = 0; i < 10 /*this->m_elements.size()*/; i++) {
    os << indent + URX_STD_INDENT << '#' << i + 1 << ": ";
    this->m_elements[i].printSelf(os, "");
  }

  os << indent << "ElementGeometries: (size=" << this->m_elementGeometries.size() << ")" << '\n';
  for (uint32_t i = 0; i < this->m_elementGeometries.size(); i++) {
    os << indent + URX_STD_INDENT << '#' << i + 1 << ": ";
    this->m_elementGeometries[i].printSelf(os, indent + URX_STD_INDENT + URX_STD_INDENT);
  }

  os << indent << "ImpulseResponses: (size=" << this->m_impulseResponses.size() << ")" << '\n';
  if (!this->m_impulseResponses.empty())
    for (uint32_t i = 0; i < this->m_impulseResponses.size(); i++) {
      os << indent + URX_STD_INDENT << '#' << i + 1 << ':' << '\n';
      this->m_impulseResponses[i].printSelf(os, indent + URX_STD_INDENT + URX_STD_INDENT);
    }
}

}  // namespace urx::v0_2