/*!
 * Copyright Moduleus
 * \file urx/event.h
 * \brief
 */

#ifndef URX_EVENT_H
#define URX_EVENT_H

#include <iosfwd>
#include <string>

#include <urx/v0_2/object.h>
#include <urx/v0_2/receive_setup.h>
#include <urx/v0_2/transmit_setup.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief URX class to describe an unique ultrasound event, composed by a single transmit and receive setup
 */
class Event : public Object {
  URX_TYPE_MACRO(Event, Object);

 public:
  Event() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  TransmitSetup& transmitSetup() { return m_transmitSetup; }
  void setTransmitSetup(const TransmitSetup& transmitSetup) { m_transmitSetup = transmitSetup; }

  ReceiveSetup& receiveSetup() { return m_receiveSetup; }
  void setReceiveSetup(const ReceiveSetup& receiveSetup) { m_receiveSetup = receiveSetup; }

  bool operator==(const Event& other) const {
    return ((m_transmitSetup == other.m_transmitSetup) && (m_receiveSetup == other.m_receiveSetup));
  }
  inline bool operator!=(const Event& other) const { return !(*this == other); }

 private:
  // Description of the transmit event (probe/channels, waves, excitations, etc.). If more than one probe is used in reception, this is a list of setups.
  TransmitSetup m_transmitSetup;

  // Description of sampled channel data (probe/channels, sampling, TGC, etc.). If more than one probe is used in reception, this is a list of setups.
  ReceiveSetup m_receiveSetup;
};

}  // namespace urx::v0_2

#endif  // URX_EVENT_H