/*!
 * Copyright Moduleus
 * \file uff/dataset.cpp
 * \brief
 */

#include "uff/dataset.h"

namespace uff {

void Dataset::printSelf(std::ostream& os, std::string indent) const {
  superclass::printSelf(os, indent);
  os << indent << "ChannelData: ";
  m_channelData.printSelf(os, indent + UFF_STD_INDENT);
  os << indent << "Version: ";
  this->version().printSelf(os, indent + UFF_STD_INDENT);
}

}  // namespace uff
