/*!
 * Copyright Moduleus
 * \file uff/dataset.cpp
 * \brief
 */

#include "uff/dataset.h"

#include <ostream>

namespace uff {

template <typename DataType>
void Dataset<DataType>::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "ChannelData: ";
  m_channelData.printSelf(os, indent + UFF_STD_INDENT);
  os << indent << "Version: ";
  this->version().printSelf(os, indent + UFF_STD_INDENT);
}

template <typename DataType>
Dataset<DataType>& Dataset<DataType>::operator=(const Dataset<DataType>& other) {
  if (this == &other) {
    return *this;
  }

  // Copy version
  this->setVersion(other.version());

  // Copy channel data
  this->setChannelData(other.channelData());

  return *this;
}

template class Dataset<float>;
template class Dataset<short>;

}  // namespace uff
