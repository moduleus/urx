#pragma once

#include <uff/uff.h>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace uff {

/**
 * @brief The UFF ImpulseResponse class specifies a temporal impulse response
 */
class ImpulseResponse {
  // CTOR & DTOR
 public:
  ImpulseResponse() = default;
  ImpulseResponse(const ImpulseResponse&) = default;
  ImpulseResponse(ImpulseResponse&&) = default;
  ~ImpulseResponse() = default;

  // Operators
 public:
  ImpulseResponse& operator=(const ImpulseResponse& other) noexcept = default;
  ImpulseResponse& operator=(ImpulseResponse&& other) noexcept = default;
  inline bool operator==(const ImpulseResponse& other) const {
    return ((m_initialTime == other.m_initialTime) &&
            (_sampling_frequency == other._sampling_frequency) && (m_data == other.m_data) &&
            (m_units == other.m_units));
  }
  inline bool operator!=(const ImpulseResponse& other) const { return !(*this == other); }

  // Accessors
 public:
  inline MetadataType initialTime() const { return m_initialTime; }
  inline void setInitialTime(MetadataType initialTime) { m_initialTime = initialTime; }

  inline std::optional<MetadataType> samplingFrequency() const { return _sampling_frequency; }
  inline void setSampleFrequency(std::optional<MetadataType> samplingFrequency) {
    _sampling_frequency = samplingFrequency;
  }

  inline const std::vector<MetadataType>& data() const { return m_data; }
  inline void setData(const std::vector<MetadataType>& data) { m_data = data; }

  inline const std::string& units() const { return m_units; }
  inline void setUnits(const std::string& units) { m_units = units; }

  // Members
 private:
  // Time in seconds from the delta excitation until the acquisition of the first sample
  MetadataType m_initialTime = 0.f;

  // Sampling frequency in Hz
  std::optional<MetadataType> _sampling_frequency = std::nullopt;

  // Collection of samples containing the impulse response
  std::vector<MetadataType> m_data;

  // (Optional) Name of the units of the impulse response
  std::string m_units;
};

}  // namespace uff
