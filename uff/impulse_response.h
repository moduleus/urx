#pragma once

#include <uff/time_offset_base.h>
#include <uff/uff.h>

#include <string>
#include <vector>

namespace uff {

/**
 * @brief The UFF ImpulseResponse class specifies a temporal impulse response
 */
class ImpulseResponse : public TimeOffsetBase {
 public:
  // CTOR & DTOR
  ImpulseResponse() = delete;
  ImpulseResponse(MetadataType sampling_frequency, const std::vector<MetadataType>& data = {},
                  MetadataType time_offset = 0., const std::string& units = "")
      : TimeOffsetBase(time_offset),
        _sampling_frequency(sampling_frequency),
        _data(data),
        _units(units) {}
  ImpulseResponse(const ImpulseResponse&) = default;
  ImpulseResponse(ImpulseResponse&&) = default;
  ~ImpulseResponse() override = default;

  // Operators
  ImpulseResponse& operator=(const ImpulseResponse& other) noexcept = default;
  ImpulseResponse& operator=(ImpulseResponse&& other) noexcept = default;
  inline bool operator==(const ImpulseResponse& other) const {
    return (TimeOffsetBase::operator==(other) &&
            (_sampling_frequency == other._sampling_frequency) && (_data == other._data) &&
            (_units == other._units));
  }
  inline bool operator!=(const ImpulseResponse& other) const { return !(*this == other); }

  // Accessors
  inline MetadataType samplingFrequency() const { return _sampling_frequency; }
  inline void setSampleFrequency(MetadataType sampling_frequency) {
    _sampling_frequency = sampling_frequency;
  }

  inline const std::vector<MetadataType>& data() const { return _data; }
  inline void setData(const std::vector<MetadataType>& data) { _data = data; }

  inline const std::string& units() const { return _units; }
  inline void setUnits(const std::string& units) { _units = units; }

  // Members
 private:
  // Sampling frequency in Hz
  MetadataType _sampling_frequency;

  // Collection of samples containing the impulse response
  std::vector<MetadataType> _data;

  // (Optional) Name of the units of the impulse response
  std::string _units;
};

}  // namespace uff
