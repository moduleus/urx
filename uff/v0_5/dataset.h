/*!
 * Copyright Moduleus
 * \file uff/dataset.h
 * \brief
 */

#ifndef UFF_DATASET_H
#define UFF_DATASET_H

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "uff/v0_5/channel_data.h"
#include "uff/v0_5/event.h"
#include "uff/v0_5/excitation.h"
#include "uff/v0_5/object.h"
#include "uff/v0_5/receive_setup.h"
#include "uff/v0_5/uff.h"
#include "uff/v0_5/version.h"
#include "uff/v0_5/wave.h"

namespace uff::v0_5 {

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
      return UFF_NAN;
    }
    return m_channelData.uniqueEvents()[0]->receiveSetup().timeOffset();
  }

  // Returns the type of sampling of the 1st ReceiveSetup
  uff::ReceiveSetup::SAMPLING_TYPE getSamplingType() const {
    if (m_channelData.uniqueEvents().empty()) {
      return uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF;
    }
    return m_channelData.uniqueEvents()[0]->receiveSetup().samplingType();
  }

  // Return the sampling frequency associated with the 1st receive event [Hz]
  MetadataType getSamplingFrequency() const {
    if (m_channelData.uniqueEvents().empty()) {
      return UFF_NAN;
    }
    return m_channelData.uniqueEvents()[0]->receiveSetup().samplingFrequency();
  }

  // Returns the speed of sound [m/s]
  MetadataType getSoundSpeed() const { return m_channelData.soundSpeed(); }

  // Return the transmit frequency associated with the 1st Wave of the dataset
  MetadataType getTransmitFrequency() const {
    if (m_channelData.uniqueWaves().empty() ||
        !m_channelData.uniqueWaves()[0]->excitation().transmitFrequency().has_value()) {
      return UFF_NAN;
    }
    return m_channelData.uniqueWaves()[0]->excitation().transmitFrequency().value();
  }

  // Returns true is the 1st probe is of sub-type 'ProbeType'
  // Example: isProbeType<uff::MatrixArray>() == true;
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
  uff::Version m_version;
  uff::ChannelData<DataType> m_channelData;
};

}  // namespace uff::v0_5

#endif  // UFF_DATASET_H
