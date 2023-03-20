/*!
 * Copyright Moduleus
 * \file uff/dataset.h
 * \brief
 */

#ifndef UFF_DATASET_H
#define UFF_DATASET_H

#include <uff/channel_data.h>
#include <uff/object.h>
#include <uff/receive_setup.h>
#include <uff/uff.h>
#include <uff/version.h>
#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

namespace uff {

/**
 * @brief The UFF Dataset class
 */
template <typename DataType>
class Dataset : public uff::Object {
  UFF_TYPE_MACRO(Dataset, uff::Object);

 public:
  Dataset() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  uff::ChannelData<DataType>& channelData() { return m_channelData; }
  const uff::ChannelData<DataType>& channelData() const { return m_channelData; }
  void setChannelData(const uff::ChannelData<DataType>& channelData) {
    m_channelData = channelData;
  }
  void setChannelData(uff::ChannelData<DataType>&& channelData) {
    m_channelData = std::move(channelData);
  }

  const uff::Version& version() const { return m_version; }
  void setVersion(const uff::Version& version) { m_version = version; }

  // ___________________ Convenience access method ___________________________________

  // Returns the channel geometry of the probe used by the 1st receive setup
  template <typename T>
  std::vector<T> getChannelGeometry() const {
    return m_channelData.template channelGeometry<T>();
  }

  // Returns the receive delay of the 1st ReceiveSetup
  MetadataType getReceiveDelay() const { return m_channelData.receiveDelay(); }

  // Returns the type of sampling of the 1st ReceiveSetup
  uff::ReceiveSetup::SAMPLING_TYPE getSamplingType() const { return m_channelData.samplingType(); }

  // Return the sampling frequency associated with the 1st receive event [Hz]
  MetadataType getSamplingFrequency() const { return m_channelData.samplingFrequency(); }

  // Returns the speed of sound [m/s]
  MetadataType getSoundSpeed() const { return m_channelData.soundSpeed(); }

  // Return the transmit frequency associated with the 1st Wave of the dataset
  MetadataType getTransmitFrequency() const { return m_channelData.transmitFrequency(); }

  // Returns true is the 1st probe is of sub-type 'ProbeType'
  // Example: isProbeType<uff::MatrixArray>() == true;
  template <class ProbeType>
  bool isProbeType() const {
    return m_channelData.template isProbeType<ProbeType>();
  }

  inline bool operator==(const Dataset& other) const {
    return ((m_version == other.m_version) && (m_channelData == other.m_channelData));
  }

  inline bool operator!=(const Dataset& other) const { return !(*this == other); }

 private:
  uff::Version m_version;
  uff::ChannelData<DataType> m_channelData;
};

}  // namespace uff

#endif  // UFF_DATASET_H
