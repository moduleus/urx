/*!
 * Copyright Moduleus
 * \file uff/dataset.cpp
 * \brief
 */

#include "uff/dataset.h"

namespace uff {

template <typename DataType>
void Dataset<DataType>::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "ChannelData: ";
  m_channelData.printSelf(os, indent + UFF_STD_INDENT);
  os << indent << "Version: ";
  this->version().printSelf(os, indent + UFF_STD_INDENT);
}

template class Dataset<float>;
template class Dataset<short>;

}  // namespace uff
