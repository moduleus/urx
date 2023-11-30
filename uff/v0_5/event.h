/*!
 * Copyright Moduleus
 * \file uff/event.h
 * \brief
 */

#ifndef UFF_EVENT_H
#define UFF_EVENT_H

#include <iosfwd>
#include <string>
#include "uff/v0_5/object.h"
#include "uff/v0_5/receive_setup.h"
#include "uff/v0_5/transmit_setup.h"
#include "uff/v0_5/uff.h"

namespace uff::v0_5 {

/**
 * @brief UFF class to describe an unique ultrasound event, composed by a single transmit and receive setup
 */
class Event : public uff::Object {
  UFF_TYPE_MACRO(Event, uff::Object);

 public:
  Event() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

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

}  // namespace uff::v0_5

#endif  // UFF_EVENT_H