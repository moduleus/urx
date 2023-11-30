/*!
 * Copyright Moduleus
 * \file urx/channel_data.cpp
 * \brief
 */

#include <urx/v0_5/channel_data.h>

#include <iomanip>
#include <ostream>
#include <utility>

#include <urx/v0_5/log.h>
#include <urx/v0_5/receive_setup.h>
#include <urx/v0_5/transmit_setup.h>
#include <urx/v0_5/transmit_wave.h>

namespace urx::v0_5 {

template <typename DataType>
void ChannelData<DataType>::printSelf(std::ostream& os, const std::string& indent) const {
  superclass::printSelf(os, indent);
  os << indent << "Authors: " << '"' << authors() << '"' << std::endl;
  os << indent << "Description: " << '"' << description() << '"' << std::endl;
  os << indent << "LocalTime: " << '"' << localTime() << '"' << std::endl;
  os << indent << "CountryCode: " << '"' << countryCode() << '"' << std::endl;
  os << indent << "System: " << '"' << system() << '"' << std::endl;
  os << indent << "SoundSpeed: " << m_soundSpeed << std::endl;

  if (m_repetitionRate.has_value()) {
    os << indent << "RepetitionRate: " << m_repetitionRate.value() << std::endl;
  } else {
    os << indent << "RepetitionRate: " << UNDEFINED << std::endl;
  }

  os << indent << "Probes: (size=" << probes().size() << ")" << std::endl;

  for (uint32_t i = 0; i < m_probes.size(); i++) {
    const auto& sptr = probes()[i];
    os << indent + URX_STD_INDENT << '#' << std::setfill('0') << std::setw(8) << i + 1 << ": ";
    (*sptr).printSelf(os, indent + URX_STD_INDENT + URX_STD_INDENT);
  }

  os << indent << "UniqueWaves: (size=" << m_uniqueWaves.size() << ")" << std::endl;
  for (uint32_t i = 0; i < uniqueWaves().size(); i++) {
    os << indent + URX_STD_INDENT << "#" << std::setfill('0') << std::setw(8) << i + 1 << ": ";
    const auto& sptr = uniqueWaves()[i];
    (*sptr).printSelf(os, indent + URX_STD_INDENT + URX_STD_INDENT);
  }

  os << indent << "UniqueEvents: (size=" << m_uniqueEvents.size() << ")" << std::endl;
  for (uint32_t i = 0; i < uniqueEvents().size(); i++) {
    os << indent + URX_STD_INDENT << "#" << std::setfill('0') << std::setw(8) << i + 1 << ": ";
    const auto& sptr = uniqueEvents()[i];
    (*sptr).printSelf(os, indent + URX_STD_INDENT + URX_STD_INDENT);
  }

  os << indent << "Sequence: (size=" << m_sequence.size() << ")" << std::endl;
  for (uint32_t i = 0; i < sequence().size(); i++) {
    os << indent + URX_STD_INDENT << "#" << std::setfill('0') << std::setw(8) << i + 1 << ": ";
    const auto& timedEvent = sequence()[i];
    timedEvent.printSelf(os, indent + URX_STD_INDENT + URX_STD_INDENT);
  }

  os << indent << "Data: (size=" << m_data.size() << ")\n" << std::endl;
}

template <typename DataType>
ChannelData<DataType>& ChannelData<DataType>::copyStructure(const ChannelData<DataType>& other) {
  m_authors = other.m_authors;
  m_description = other.m_description;
  m_localTime = other.m_localTime;
  m_countryCode = other.m_countryCode;
  m_system = other.m_system;
  m_soundSpeed = other.m_soundSpeed;
  m_repetitionRate = other.m_repetitionRate;

  // Probe
  for (auto& probe : other.m_probes) {
    std::shared_ptr<urx::Probe> newProbe = probe->clone();
    m_probes.push_back(newProbe);
    if (*newProbe != *probe) throw;
    if (m_probes.back() == probe) throw;
  }

  // Waves
  for (auto& uniqueWave : other.m_uniqueWaves) {
    m_uniqueWaves.push_back(std::make_shared<urx::Wave>(*uniqueWave));
    if (*m_uniqueWaves.back() != *uniqueWave) throw;
    if (m_uniqueWaves.back() == uniqueWave) throw;
  }

  // Events
  for (auto& uniqueEvent : other.m_uniqueEvents) {
    m_uniqueEvents.push_back(std::make_shared<Event>());
    *m_uniqueEvents.back() = *uniqueEvent;

    // Transmit Probe weak pointer
    {
      std::shared_ptr<urx::Probe> transmitProbe;
      for (size_t iProbe = 0; iProbe < m_probes.size() && !transmitProbe; iProbe++) {
        if (*m_probes[iProbe] == *uniqueEvent->transmitSetup().probe().lock()) {
          transmitProbe = m_probes[iProbe];
        }
      }
      m_uniqueEvents.back()->transmitSetup().setProbe(transmitProbe);
      bool foundTransmitProbe =
          std::find(m_probes.begin(), m_probes.end(),
                    m_uniqueEvents.back()->transmitSetup().probe().lock()) != m_probes.end();
      if (!foundTransmitProbe) throw;
    }

    // Receive Probe weak pointer
    {
      std::shared_ptr<urx::Probe> receiveProbe;
      for (size_t iProbe = 0; iProbe < m_probes.size() && !receiveProbe; iProbe++) {
        if (*m_probes[iProbe] == *uniqueEvent->receiveSetup().probe().lock()) {
          receiveProbe = m_probes[iProbe];
        }
      }
      m_uniqueEvents.back()->receiveSetup().setProbe(receiveProbe);
      bool foundReceiveProbe =
          std::find(m_probes.begin(), m_probes.end(),
                    m_uniqueEvents.back()->receiveSetup().probe().lock()) != m_probes.end();
      if (!foundReceiveProbe) throw;
    }

    // Wave weak pointer
    {
      std::shared_ptr<urx::Wave> wave;
      for (size_t iWave = 0; iWave < m_uniqueWaves.size() && !wave; iWave++) {
        if (*m_uniqueWaves[iWave] == *uniqueEvent->transmitSetup().transmitWave().wave().lock()) {
          wave = m_uniqueWaves[iWave];
        }
      }
      m_uniqueEvents.back()->transmitSetup().transmitWave().setWave(wave);
      bool foundWave =
          std::find(m_uniqueWaves.begin(), m_uniqueWaves.end(),
                    m_uniqueEvents.back()->transmitSetup().transmitWave().wave().lock()) !=
          m_uniqueWaves.end();
      if (!foundWave) throw;
    }

    if (*m_uniqueEvents.back() != *uniqueEvent) throw;
    if (m_uniqueEvents.back() == uniqueEvent) throw;
  }

  // Sequence
  for (auto& timedEvent : other.m_sequence) {
    m_sequence.push_back(timedEvent);

    // Event weak pointer
    std::shared_ptr<urx::Event> evenement;
    for (size_t iEvent = 0; iEvent < m_uniqueEvents.size() && !evenement; iEvent++) {
      if (*m_uniqueEvents[iEvent] == *timedEvent.evenement().lock()) {
        evenement = m_uniqueEvents[iEvent];
      }
    }
    m_sequence.back().setEvent(evenement);
    if (m_sequence.back() != timedEvent) throw;
  }

  // Sizes
  m_numberOfFrames = other.m_numberOfFrames;
  m_numberOfEvents = other.m_numberOfEvents;
  m_numberOfChannels = other.m_numberOfChannels;
  m_numberOfSamples = other.m_numberOfSamples;

  return *this;
}

template <typename DataType>
ChannelData<DataType>& ChannelData<DataType>::operator=(const ChannelData<DataType>& other) {
  if (&other == this) return *this;

  copyStructure(other);

  // Data
  m_data = other.m_data;

  const bool differents = *this != other;
  if (differents) throw;

  return *this;
}

template <typename DataType>
void ChannelData<DataType>::setLocalTime(const std::string& localTime) {
  // validate
  if (!localTime.empty() && isIso8601(localTime)) {
    m_localTime = localTime;
  } else {
    LOG_THIS(ERROR) << '"' << localTime << "\" is not ISO8601 format (YYYY-MM-DDThh:mm:ss)\n";
  }
}

template <typename DataType>
void ChannelData<DataType>::setCountryCode(const std::string& countryCode) {
  // validate
  if (!countryCode.empty() && isIso3166(countryCode)) {
    m_countryCode = countryCode;
  } else {
    LOG_THIS(ERROR) << '"' << countryCode << "\" is not ISO3166 (XX)\n";
  }
}

template class ChannelData<float>;
template class ChannelData<short>;

}  // namespace urx::v0_5
