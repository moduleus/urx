/*!
 * Copyright Moduleus
 * \file uff_event.h
 * \brief
 */

#ifndef UFF_EVENT_H
#define UFF_EVENT_H

// UFF
#include "uff_object.h"
#include "uff_receive_setup.h"
#include "uff_transmit_setup.h"

namespace uff {

/**
 * @brief UFF class to describe an unique ultrasound event, composed by a single transmit and receive setup
 */
class Event : public uff::Object {
  UFF_TYPE_MACRO(Event, uff::Object);

 public:
  Event() {}

  void printSelf(std::ostream& os, std::string indent) const override;

  uff::TransmitSetup& transmitSetup() { return m_transmitSetup; }
  void setTransmitSetup(const uff::TransmitSetup& transmitSetup) {
    m_transmitSetup = transmitSetup;
  }

  uff::ReceiveSetup& receiveSetup() { return m_receiveSetup; }
  void setReceiveSetup(const uff::ReceiveSetup& receiveSetup) { m_receiveSetup = receiveSetup; }

  bool operator==(const Event& other) const {
    return ((m_transmitSetup == other.m_transmitSetup) && (m_receiveSetup == other.m_receiveSetup));
  }
  inline bool operator!=(const Event& other) const { return !(*this == other); }

 private:
  // Description of the transmit event (probe/channels, waves, excitations, etc.). If more than one probe is used in reception, this is a list of setups.
  uff::TransmitSetup m_transmitSetup;

  // Description of sampled channel data (probe/channels, sampling, TGC, etc.). If more than one probe is used in reception, this is a list of setups.
  uff::ReceiveSetup m_receiveSetup;
};

}  // namespace uff

#endif  // UFF_EVENT_H