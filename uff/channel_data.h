/*!
 * Copyright Moduleus
 * \file uff/channel_data.h
 * \brief
 */

#ifndef UFF_CHANNEL_DATA_H
#define UFF_CHANNEL_DATA_H

#include <uff/event.h>
#include <uff/object.h>
#include <uff/probe.h>
#include <uff/timed_event.h>
#include <uff/uff.h>
#include <uff/wave.h>
#include <algorithm>
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
#include <vector>
namespace uff {

/**
 * @brief UFF class that contains all the information needed to store and later process channel data.
 */
template <typename DataType>
class ChannelData : public uff::Object {
  UFF_TYPE_MACRO(ChannelData, uff::Object);

 public:
  ChannelData() = default;
  ChannelData(const ChannelData&) = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  // Authors
  const std::string& authors() const { return m_authors; }
  void setAuthors(const std::string& authors) { m_authors = authors; }

  // Description
  const std::string& description() const { return m_description; }
  void setDescription(const std::string& description) { m_description = description; }

  /**
     * Format: ISO8601
     * Example: 
     *   "2008-09-15T15:53:00"
     *   "2008-09-15T15:53:00+05:00"
     */
  const std::string& localTime() const { return m_localTime; }
  void setLocalTime(const std::string& localTime);

  /**
     * Format: ISO3166-1
     * Example:
     *     "FR" for France
     */
  const std::string& countryCode() const { return m_countryCode; }
  void setCountryCode(const std::string& countryCode);

  // 'System' describes the acquisition system used to acquire the data
  const std::string& system() const { return m_system; }
  void setSystem(const std::string& system) { m_system = system; }

  // Speed of sound in m/s
  MetadataType soundSpeed() const { return m_soundSpeed; }
  void setSoundSpeed(MetadataType soundSpeed) { m_soundSpeed = soundSpeed; }

  // Sequence repetition rate in Hz. Sometimes called framerate.
  std::optional<MetadataType> repetitionRate() const { return m_repetitionRate; }
  void setRepetitionRate(std::optional<MetadataType> repetitionRate) {
    m_repetitionRate = repetitionRate;
  }

  // List of probes used for this dataset
  const std::vector<std::shared_ptr<uff::Probe>>& probes() const { return m_probes; }
  void addProbe(const std::shared_ptr<uff::Probe>& probe) { m_probes.push_back(probe); }
  void setProbes(const std::vector<std::shared_ptr<uff::Probe>>& probes) { m_probes = probes; }

  // List of unique waves used for this dataset
  const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves() const { return m_uniqueWaves; }
  void addUniqueWave(const std::shared_ptr<uff::Wave>& wave) { m_uniqueWaves.push_back(wave); }
  void setUniqueWaves(const std::vector<std::shared_ptr<uff::Wave>>& uniqueWaves) {
    m_uniqueWaves = uniqueWaves;
  }

  /* List of unique events used for this dataset */
  const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents() const { return m_uniqueEvents; }
  void addUniqueEvent(const std::shared_ptr<uff::Event>& event) { m_uniqueEvents.push_back(event); }
  void setUniqueEvents(const std::vector<std::shared_ptr<uff::Event>>& uniqueEvents) {
    m_uniqueEvents = uniqueEvents;
  }

  const std::vector<uff::TimedEvent>& sequence() const { return m_sequence; }
  void addTimedEvent(const uff::TimedEvent& timedEvent) { m_sequence.push_back(timedEvent); }
  void setSequence(const std::vector<uff::TimedEvent>& sequence) { m_sequence = sequence; }

  // data
  std::vector<DataType>& data() {
    if (m_skipChannelDataData) {
      throw std::logic_error("Can't read data.");
    }
    return m_data;
  }
  const std::vector<DataType>& data() const {
    if (m_skipChannelDataData) {
      throw std::logic_error("Can't read data.");
    }
    return m_data;
  }
  void setData(const std::vector<DataType>& data) { m_data = data; }

  const DataType* dataAt(int frame, int event, int channel) const {
    return m_data.data() +
           (static_cast<size_t>(frame) * numberOfEvents() * numberOfChannels() *
            numberOfSamples()) +
           (static_cast<size_t>(event) * numberOfChannels() * numberOfSamples()) +
           (static_cast<size_t>(channel) * numberOfSamples());
  }
  DataType* dataAt(int frame, int event, int channel) {
    return m_data.data() +
           (static_cast<size_t>(frame) * numberOfEvents() * numberOfChannels() *
            numberOfSamples()) +
           (static_cast<size_t>(event) * numberOfChannels() * numberOfSamples()) +
           (static_cast<size_t>(channel) * numberOfSamples());
  }

  uint32_t numberOfFrames() const { return m_numberOfFrames; }
  uint32_t numberOfEvents() const { return m_numberOfEvents; }
  uint32_t numberOfChannels() const { return m_numberOfChannels; }
  uint32_t numberOfSamples() const { return m_numberOfSamples; }

  void setNumberOfFrames(uint32_t sz) { m_numberOfFrames = sz; }
  void setNumberOfEvents(uint32_t sz) { m_numberOfEvents = sz; }
  void setNumberOfChannels(uint32_t sz) { m_numberOfChannels = sz; }
  void setNumberOfSamples(uint32_t sz) { m_numberOfSamples = sz; }

  void setSkipChannelDataData(bool skip) { m_skipChannelDataData = skip; }

  void allocate() {
    size_t sz = static_cast<size_t>(numberOfFrames()) * numberOfEvents() * numberOfChannels() *
                numberOfSamples();
    m_data.resize(sz, 0);
  }

  /**
     * Returns true if the input string is ISO8601
     * Example: "2008-09-15T15:53:00"
     */
  static bool isIso8601(const std::string& dateTime) {
    const std::string r =
        R"(^([\+-]?\d{4}(?!\d{2}\b))((-?)((0[1-9]|1[0-2])(\3([12]\d|0[1-9]|3[01]))?|W([0-4]\d|5[0-2])(-?[1-7])?|(00[1-9]|0[1-9]\d|[12]\d{2}|3([0-5]\d|6[1-6])))([T\s]((([01]\d|2[0-3])((:?)[0-5]\d)?|24\:?00)([\.,]\d+(?!:))?)?(\17[0-5]\d([\.,]\d+)?)?([zZ]|([\+-])([01]\d|2[0-3]):?([0-5]\d)?)?)?)?$)";
    return std::regex_match(dateTime, std::regex(r));
  }

  /**
     * Returns true if the input string is ISO3166
     * Example: "FR"
     * Note: only checks that it is a 2-letter uppercase
     */
  static bool isIso3166(const std::string& countryCode) {
    const std::string r = R"(^[A-Z][A-Z]$)";
    return std::regex_match(countryCode, std::regex(r));
  }

  ChannelData& operator=(const ChannelData& other);
  ChannelData& operator=(ChannelData&& other) noexcept = default;

  bool operator==(const ChannelData& other) const {
    bool areProbesEqualed = true;
    for (uint32_t i = 0; i < m_probes.size() && areProbesEqualed; ++i) {
      areProbesEqualed = areProbesEqualed && (*m_probes[i] == *other.m_probes[i]);
    }

    bool areUniqueWavesEqualed = true;
    for (uint32_t i = 0; i < m_uniqueWaves.size() && areUniqueWavesEqualed; ++i) {
      areUniqueWavesEqualed =
          areUniqueWavesEqualed && (*m_uniqueWaves[i] == *other.m_uniqueWaves[i]);
    }

    bool areUniqueEventsEqualed = true;
    for (uint32_t i = 0; i < m_uniqueEvents.size() && areUniqueEventsEqualed; ++i) {
      areUniqueEventsEqualed =
          areUniqueEventsEqualed && (*m_uniqueEvents[i] == *other.m_uniqueEvents[i]);
    }

    return ((m_authors == other.m_authors) && (m_description == other.m_description) &&
            (m_localTime == other.m_localTime) && (m_countryCode == other.m_countryCode) &&
            (m_system == other.m_system) && (m_soundSpeed == other.m_soundSpeed) &&
            (m_repetitionRate == other.m_repetitionRate) && areProbesEqualed &&
            areUniqueWavesEqualed && areUniqueEventsEqualed && (m_sequence == other.m_sequence) &&
            (m_data == other.m_data) && (m_numberOfFrames == other.m_numberOfFrames) &&
            (m_numberOfEvents == other.m_numberOfEvents) &&
            (m_numberOfSamples == other.m_numberOfSamples) &&
            (m_numberOfChannels == other.m_numberOfChannels));
  }

  inline bool operator!=(const ChannelData& other) const { return !(*this == other); }

 private:
  // string with the authors of the data
  std::string m_authors;

  // string describing the data
  std::string m_description;

  // string defining the time the dataset was acquired following ISO 8601
  std::string m_localTime;

  // string defining the country, following ISO 3166-1
  std::string m_countryCode;

  // string defining the system used to acquired the dataset
  std::string m_system;

  // Reference sound speed for Tx and Rx events [m/s]
  MetadataType m_soundSpeed = 0;

  // Inverse of the time delay between consecutive repetitions of the sequence
  std::optional<MetadataType> m_repetitionRate = std::nullopt;

  // List of the probes used to transmit/recive the sequence
  std::vector<std::shared_ptr<uff::Probe>> m_probes;

  // List of the unique waves (or beams) used in the sequence
  std::vector<std::shared_ptr<uff::Wave>> m_uniqueWaves;

  // List of the unique transmit/receive events used in the sequence
  std::vector<std::shared_ptr<uff::Event>> m_uniqueEvents;

  // List of the times_events that describe the sequence
  std::vector<uff::TimedEvent> m_sequence;

  // data[iFrame][iEvent][iChannel][iSample]
  std::vector<DataType> m_data;
  uint32_t m_numberOfFrames = 0;
  uint32_t m_numberOfEvents = 0;
  uint32_t m_numberOfChannels = 0;
  uint32_t m_numberOfSamples = 0;

  bool m_skipChannelDataData = false;
};

}  // namespace uff

#endif  // UFF_CHANNEL_DATA_H
