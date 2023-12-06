/*!
 * Copyright Moduleus
 * \file urx/impulse_response.h
 * \brief
 */

#ifndef URX_IMPULSE_RESPONSE_H
#define URX_IMPULSE_RESPONSE_H

#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <urx/v0_2/object.h>
#include <urx/v0_2/urx.h>

namespace urx::v0_2 {

/**
 * @brief The URX ImpulseResponse class specifies a temporal impulse response
 */
class ImpulseResponse : public Object {
  URX_TYPE_MACRO(ImpulseResponse, Object);

 public:
  ImpulseResponse() = default;

  void printSelf(std::ostream& os, const std::string& indent) const override;

  MetadataType initialTime() const { return m_initialTime; }
  void setInitialTime(MetadataType initialTime) { m_initialTime = initialTime; }

  std::optional<MetadataType> samplingFrequency() const { return m_samplingFrequency; }
  void setSampleFrequency(std::optional<MetadataType> samplingFrequency) {
    m_samplingFrequency = samplingFrequency;
  }

  const std::vector<MetadataType>& data() const { return m_data; }
  void setData(const std::vector<MetadataType>& data) { m_data = data; }

  const std::string& units() const { return m_units; }
  void setUnits(const std::string& units) { m_units = units; }

  bool operator==(const ImpulseResponse& other) const {
    return ((m_initialTime == other.m_initialTime) &&
            (m_samplingFrequency == other.m_samplingFrequency) && (m_data == other.m_data) &&
            (m_units == other.m_units));
  }

  inline bool operator!=(const ImpulseResponse& other) const { return !(*this == other); }

 private:
  // Time in seconds from the delta excitation until the acquisition of the first sample
  MetadataType m_initialTime = 0.f;

  // Sampling frequency in Hz
  std::optional<MetadataType> m_samplingFrequency = std::nullopt;

  // Collection of samples containing the impulse response
  std::vector<MetadataType> m_data;

  // (Optional) Name of the units of the impulse response
  std::string m_units;
};

}  // namespace urx::v0_2

#endif  // URX_IMPULSE_RESPONSE_H
