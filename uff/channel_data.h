#pragma once

#include <uff/event.h>
#include <uff/excitation.h>
#include <uff/probe.h>
#include <uff/receive_setup.h>
#include <uff/timed_event.h>
#include <uff/uff.h>
#include <uff/wave.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <ios>
#include <iosfwd>
#include <istream>
#include <locale>
#include <map>
#include <memory>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace uff {

/**
 * @brief UFF class that contains all the information needed to store and later process channel data.
 */
template <typename DataType>
class ChannelData {
  // CTOR & DTOR
 public:
  ChannelData() = default;
  ChannelData(const ChannelData&) = default;
  ChannelData(ChannelData&&) noexcept = default;
  ~ChannelData() = default;

  // Operators
 public:
  ChannelData& operator=(const ChannelData& other) noexcept = default;
  ChannelData& operator=(ChannelData&& other) noexcept = default;
  bool operator==(const ChannelData& other) const {
    bool are_probes_equaled = true;
    for (uint32_t i = 0; i < _probes.size() && are_probes_equaled; ++i) {
      are_probes_equaled = are_probes_equaled && (*_probes[i] == *other._probes[i]);
    }

    bool are_unique_waves_equaled = true;
    for (uint32_t i = 0; i < _unique_waves.size() && are_unique_waves_equaled; ++i) {
      are_unique_waves_equaled =
          are_unique_waves_equaled && (*_unique_waves[i] == *other._unique_waves[i]);
    }

    bool are_unique_events_equaled = true;
    for (uint32_t i = 0; i < _unique_events.size() && are_unique_events_equaled; ++i) {
      are_unique_events_equaled =
          are_unique_events_equaled && (*_unique_events[i] == *other._unique_events[i]);
    }

    return ((_authors == other._authors) && (_description == other._description) &&
            (_local_time == other._local_time) && (_country_code == other._country_code) &&
            (_system == other._system) && (_sound_speed == other._sound_speed) &&
            (_repetition_rate == other._repetition_rate) && are_probes_equaled &&
            are_unique_waves_equaled && are_unique_events_equaled &&
            (_sequence == other._sequence) && (_data == other._data) &&
            (_nb_frames == other._nb_frames) && (_nb_events == other._nb_events) &&
            (_nb_samples == other._nb_samples) && (_nb_channels == other._nb_channels));
  }
  inline bool operator!=(const ChannelData& other) const { return !(*this == other); }

  // Accessors
 public:
  inline const std::string& authors() const { return _authors; }
  inline void setAuthors(const std::string& authors) { _authors = authors; }

  inline const std::string& description() const { return _description; }
  inline void setDescription(const std::string& description) { _description = description; }

  /**
     * Should be at format: ISO8601
     * Example: 
     *   "2008-09-15T15:53:00"
     *   "2008-09-15T15:53:00+05:00"
     */
  inline const std::string& localTime() const { return _local_time; }
  void setLocalTime(const std::string& local_time) {
    _local_time = local_time;
  }

  /**
     * Should be at format: ISO3166-1
     * Example:
     *     "FR" for France
     */
  inline const std::string& countryCode() const { return _country_code; }
  void setCountryCode(const std::string& country_code) {
    _country_code = country_code;
  }

  // 'System' describes the acquisition system used to acquire the data
  inline const std::string& system() const { return _system; }
  inline void setSystem(const std::string& system) { _system = system; }

  // Speed of sound in m/s
  inline MetadataType soundSpeed() const { return _sound_speed; }
  inline void setSoundSpeed(MetadataType soundSpeed) { _sound_speed = soundSpeed; }

  // Returns the channel geometry of the probe used by the 1st receive setup
  template <typename T>
  std::vector<T> channelGeometry() const {
    if (probes().empty()) {
      return std::vector<T>();
    }
    if constexpr (std::is_same<T, MetadataType>::value) {
      return probes()[0]->channelGeometry();
    } else {
      auto& channelGeometry = probes()[0]->channelGeometry();
      return std::vector<T>(channelGeometry.begin(), channelGeometry.end());
    }
  }

  MetadataType receiveDelay() const {
    if (uniqueEvents().empty()) {
      return UFF_NAN;
    }
    return uniqueEvents()[0]->receiveSetup().timeOffset();
  }

  // Returns the type of sampling of the 1st ReceiveSetup
  ReceiveSetup::SAMPLING_TYPE samplingType() const {
    if (uniqueEvents().empty()) {
      return ReceiveSetup::SAMPLING_TYPE::DIRECT_RF;
    }
    return uniqueEvents()[0]->receiveSetup().samplingType();
  }

  // Return the sampling frequency associated with the 1st receive event [Hz]
  MetadataType samplingFrequency() const {
    if (uniqueEvents().empty()) {
      return UFF_NAN;
    }
    return uniqueEvents()[0]->receiveSetup().samplingFrequency();
  }

  MetadataType transmitFrequency() const {
    if (uniqueWaves().empty() || !uniqueWaves()[0]->excitation().transmitFrequency().has_value()) {
      return UFF_NAN;
    }
    return uniqueWaves()[0]->excitation().transmitFrequency().value();
  }

  // Returns true is the 1st probe is of sub-type 'ProbeType'
  // Example: isProbeType<uff::MatrixArray>() == true;
  template <class ProbeType>
  bool isProbeType() const {
    if (probes().empty()) {
      return false;
    }  // Try to cast the 1st probe to the user-provided type
    std::shared_ptr<ProbeType> pt = std::dynamic_pointer_cast<ProbeType>(probes()[0]);
    return (pt.get() != nullptr);
  }

  // Sequence repetition rate in Hz. Sometimes called framerate.
  inline std::optional<MetadataType> repetitionRate() const { return _repetition_rate; }
  inline void setRepetitionRate(std::optional<MetadataType> repetitionRate) {
    _repetition_rate = repetitionRate;
  }

  // List of probes used for this dataset
  inline const std::vector<std::shared_ptr<uff::Probe>>& probes() const { return _probes; }
  inline void addProbe(const std::shared_ptr<uff::Probe>& probe) { _probes.push_back(probe); }
  inline void setProbes(const std::vector<std::shared_ptr<uff::Probe>>& probes) {
    _probes = probes;
  }

  // List of unique waves used for this dataset
  inline const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves() const {
    return _unique_waves;
  }
  inline void addUniqueWave(const std::shared_ptr<uff::Wave>& wave) {
    _unique_waves.push_back(wave);
  }
  inline void setUniqueWaves(const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves) {
    _unique_waves = uniqueWaves;
  }

  /* List of unique events used for this dataset */
  inline const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents() const {
    return _unique_events;
  }
  inline void addUniqueEvent(const std::shared_ptr<uff::Event>& event) {
    _unique_events.push_back(event);
  }
  inline void setUniqueEvents(const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents) {
    _unique_events = uniqueEvents;
  }

  inline const std::vector<uff::TimedEvent>& sequence() const { return _sequence; }
  inline void addTimedEvent(const uff::TimedEvent& timedEvent) { _sequence.push_back(timedEvent); }
  inline void setSequence(const std::vector<uff::TimedEvent>& sequence) { _sequence = sequence; }

  // data
  inline std::vector<DataType>& data() {
    if (_skip_data) {
      throw std::logic_error("Can't read data.");
    }
    return _data;
  }
  inline const std::vector<DataType>& data() const {
    if (_skip_data) {
      throw std::logic_error("Can't read data.");
    }
    return _data;
  }
  inline void setData(const std::vector<DataType>& data) { _data = data; }

  inline const DataType* dataAt(int frame, int event, int channel) const {
    return _data.data() +
           (static_cast<size_t>(frame) * numberOfEvents() * numberOfChannels() *
            numberOfSamples()) +
           (static_cast<size_t>(event) * numberOfChannels() * numberOfSamples()) +
           (static_cast<size_t>(channel) * numberOfSamples());
  }
  inline DataType* dataAt(int frame, int event, int channel) {
    return _data.data() +
           (static_cast<size_t>(frame) * numberOfEvents() * numberOfChannels() *
            numberOfSamples()) +
           (static_cast<size_t>(event) * numberOfChannels() * numberOfSamples()) +
           (static_cast<size_t>(channel) * numberOfSamples());
  }

  inline uint32_t numberOfFrames() const { return _nb_frames; }
  inline uint32_t numberOfEvents() const { return _nb_events; }
  inline uint32_t numberOfChannels() const { return _nb_channels; }
  inline uint32_t numberOfSamples() const { return _nb_samples; }
  inline std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> allNumberOf() const {
    return {_nb_samples, _nb_channels, _nb_events, _nb_frames};
  }

  inline void setNumberOfFrames(uint32_t sz) { _nb_frames = sz; }
  inline void setNumberOfEvents(uint32_t sz) { _nb_events = sz; }
  inline void setNumberOfChannels(uint32_t sz) { _nb_channels = sz; }
  inline void setNumberOfSamples(uint32_t sz) { _nb_samples = sz; }

 // MOVE THIS
  size_t numberOfSamplesPerEvent() const {
    return static_cast<size_t>(numberOfChannels()) * numberOfSamples();
  }
  size_t numberOfSamplesPerFrame() const {
    return static_cast<size_t>(numberOfEvents()) * numberOfChannels() * numberOfSamples();
  }
  size_t totalNumberOfSamples() const {
    return static_cast<size_t>(numberOfFrames()) * numberOfEvents() * numberOfChannels() *
           numberOfSamples();
  }

  // Distance from the prob to the start of the acquisition
  double acqStartDist() const { return receiveDelay() * soundSpeed() / 2.; }
  double acqStartToEndDist() const {
    return numberOfSamples() * soundSpeed() / samplingFrequency() / 2.0;
  }
  // Distance from the prob to the end of the acquisition
  double acqEndDist() const { return acqStartDist() + acqStartToEndDist(); }

  double sampleToLength() const { return soundSpeed() / samplingFrequency() / 2.; }
  uint32_t lengthToSample(double distance) const {
    return static_cast<uint32_t>(std::round(distance / soundSpeed() * samplingFrequency() * 2.));
  }
 // END MOVE THIS

  inline void setSkipChannelDataData(bool skip) { _skip_data = skip; }

  void allocate() {
    size_t sz = static_cast<size_t>(numberOfFrames()) * numberOfEvents() * numberOfChannels() *
                numberOfSamples();
    _data.resize(sz, 0);
  }

  // Members
 private:
  // string with the authors of the data
  std::string _authors;

  // string describing the data
  std::string _description;

  // string defining the time the dataset was acquired following ISO 8601
  std::string _local_time;

  // string defining the country, following ISO 3166-1
  std::string _country_code;

  // string defining the system used to acquired the dataset
  std::string _system;

  // Reference sound speed for Tx and Rx events [m/s]
  MetadataType _sound_speed = 0;

  // Inverse of the time delay between consecutive repetitions of the sequence
  std::optional<MetadataType> _repetition_rate = std::nullopt;

  // List of the probes used to transmit/recive the sequence
  std::vector<std::shared_ptr<uff::Probe>> _probes;

  // List of the unique waves (or beams) used in the sequence
  std::vector<std::shared_ptr<uff::Wave>> _unique_waves;

  // List of the unique transmit/receive events used in the sequence
  std::vector<std::shared_ptr<uff::Event>> _unique_events;

  // List of the times_events that describe the sequence
  std::vector<uff::TimedEvent> _sequence;

  // data[iFrame][iEvent][iChannel][iSample]
  std::vector<DataType> _data;
  uint32_t _nb_frames = 0;
  uint32_t _nb_events = 0;
  uint32_t _nb_channels = 0;
  uint32_t _nb_samples = 0;

  bool _skip_data = false;
};

template class ChannelData<float>;
template class ChannelData<short>;

}  // namespace uff
