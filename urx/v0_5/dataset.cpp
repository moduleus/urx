/*!
 * Copyright Moduleus
 * \file urx/dataset.cpp
 * \brief
 */

#include <urx/v0_5/dataset.h>

#include <ostream>

namespace urx::v0_5 {

template <typename DataType>
void Dataset<DataType>::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "ChannelData: ";
  m_channelData.printSelf(os, indent + URX_STD_INDENT);
  os << indent << "Version: ";
  this->version().printSelf(os, indent + URX_STD_INDENT);
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

}  // namespace urx::v0_5
