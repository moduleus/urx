#pragma once

#include <uff/channel_data.h>
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
class Dataset {
  // CTOR & DTOR
 public:
  Dataset() = default;
  Dataset(const Dataset&) = default;
  Dataset(Dataset&&) = default;
  ~Dataset() = default;

  // Operators
 public:
  Dataset& operator=(const Dataset& other) noexcept = default;
  Dataset& operator=(Dataset&& other) noexcept = default;
  inline bool operator==(const Dataset& other) const {
    return ((_version == other._version) && (_channel_data == other._channel_data));
  }
  inline bool operator!=(const Dataset& other) const { return !(*this == other); }

  // Accessors
 public:
  inline uff::ChannelData<DataType>& channelData() { return _channel_data; }
  inline const uff::ChannelData<DataType>& channelData() const { return _channel_data; }
  inline void setChannelData(const uff::ChannelData<DataType>& channel_data) {
    _channel_data = channel_data;
  }
  inline void setChannelData(uff::ChannelData<DataType>&& channel_data) {
    _channel_data = std::move(channel_data);
  }

  inline const uff::Version& version() const { return _version; }
  inline void setVersion(const uff::Version& version) { _version = version; }

  // Returns the channel geometry of the probe used by the 1st receive setup
  template <typename T>
  std::vector<T> getChannelGeometry() const {
    return _channel_data.template channelGeometry<T>();
  }

  // Returns the receive delay of the 1st ReceiveSetup
  MetadataType getReceiveDelay() const { return _channel_data.receiveDelay(); }

  // Returns the type of sampling of the 1st ReceiveSetup
  uff::ReceiveSetup::SAMPLING_TYPE getSamplingType() const { return _channel_data.samplingType(); }

  // Return the sampling frequency associated with the 1st receive event [Hz]
  MetadataType getSamplingFrequency() const { return _channel_data.samplingFrequency(); }

  // Returns the speed of sound [m/s]
  inline MetadataType getSoundSpeed() const { return _channel_data.soundSpeed(); }

  // Return the transmit frequency associated with the 1st Wave of the dataset
  MetadataType getTransmitFrequency() const { return _channel_data.transmitFrequency(); }

  // Returns true is the 1st probe is of sub-type 'ProbeType'
  // Example: isProbeType<uff::MatrixArray>() == true;
  template <class ProbeType>
  bool isProbeType() const {
    return _channel_data.template isProbeType<ProbeType>();
  }

  // Members
 private:
  uff::Version _version;
  uff::ChannelData<DataType> _channel_data;
};

template class Dataset<float>;
template class Dataset<short>;

}  // namespace uff
