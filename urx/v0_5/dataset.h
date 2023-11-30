/*!
 * Copyright Moduleus
 * \file urx/dataset.h
 * \brief
 */

#ifndef URX_DATASET_H
#define URX_DATASET_H

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <urx/v0_5/channel_data.h>
#include <urx/v0_5/event.h>
#include <urx/v0_5/excitation.h>
#include <urx/v0_5/object.h>
#include <urx/v0_5/receive_setup.h>
#include <urx/v0_5/urx.h>
#include <urx/v0_5/version.h>
#include <urx/v0_5/wave.h>

namespace urx::v0_5 {

/**
 * @brief The URX Dataset class
 */
template <typename DataType>
class Dataset : public Object {
  URX_TYPE_MACRO(Dataset, Object);

 public:
  Dataset() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  ChannelData<DataType>& channelData() { return m_channelData; }
  const ChannelData<DataType>& channelData() const { return m_channelData; }
  void setChannelData(const ChannelData<DataType>& channelData) { m_channelData = channelData; }
  void setChannelData(ChannelData<DataType>&& channelData) {
    m_channelData = std::move(channelData);
  }

  const Version& version() const { return m_version; }
  void setVersion(const Version& version) { m_version = version; }

  // ___________________ Convenience access method ___________________________________

  // Returns the channel geometry of the probe used by the 1st receive setup
  template <typename T>
  std::vector<T> getChannelGeometry() const {
    if (m_channelData.probes().empty()) {
      return std::vector<T>();
    }
    if constexpr (std::is_same<T, MetadataType>::value) {
      return m_channelData.probes()[0]->getChannelGeometry();
    } else {
      auto& channelGeometry = m_channelData.probes()[0]->getChannelGeometry();
      return std::vector<T>(channelGeometry.begin(), channelGeometry.end());
    }
  }

  // Returns the receive delay of the 1st ReceiveSetup
  MetadataType getReceiveDelay() const {
    if (m_channelData.uniqueEvents().empty()) {
      return URX_NAN;
    }
    return m_channelData.uniqueEvents()[0]->receiveSetup().timeOffset();
  }

  // Returns the type of sampling of the 1st ReceiveSetup
  ReceiveSetup::SAMPLING_TYPE getSamplingType() const {
    if (m_channelData.uniqueEvents().empty()) {
      return ReceiveSetup::SAMPLING_TYPE::DIRECT_RF;
    }
    return m_channelData.uniqueEvents()[0]->receiveSetup().samplingType();
  }

  // Return the sampling frequency associated with the 1st receive event [Hz]
  MetadataType getSamplingFrequency() const {
    if (m_channelData.uniqueEvents().empty()) {
      return URX_NAN;
    }
    return m_channelData.uniqueEvents()[0]->receiveSetup().samplingFrequency();
  }

  // Returns the speed of sound [m/s]
  MetadataType getSoundSpeed() const { return m_channelData.soundSpeed(); }

  // Return the transmit frequency associated with the 1st Wave of the dataset
  MetadataType getTransmitFrequency() const {
    if (m_channelData.uniqueWaves().empty() ||
        !m_channelData.uniqueWaves()[0]->excitation().transmitFrequency().has_value()) {
      return URX_NAN;
    }
    return m_channelData.uniqueWaves()[0]->excitation().transmitFrequency().value();
  }

  // Returns true is the 1st probe is of sub-type 'ProbeType'
  // Example: isProbeType<MatrixArray>() == true;
  template <class ProbeType>
  bool isProbeType() const {
    if (m_channelData.probes().empty()) {
      return false;
    }  // Try to cast the 1st probe to the user-provided type
    std::shared_ptr<ProbeType> pt = std::dynamic_pointer_cast<ProbeType>(m_channelData.probes()[0]);
    return (pt.get() != nullptr);
  }

  inline bool operator==(const Dataset& other) const {
    return ((m_version == other.m_version) && (m_channelData == other.m_channelData));
  }

  inline bool operator!=(const Dataset& other) const { return !(*this == other); }

  Dataset& operator=(const Dataset& other);
  Dataset& operator=(Dataset&& other) noexcept = default;

 private:
  Version m_version;
  ChannelData<DataType> m_channelData;
};

}  // namespace urx::v0_5

#endif  // URX_DATASET_H
