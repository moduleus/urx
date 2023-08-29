#pragma once

#include <uff/uff.h>

#include <string>
#include <vector>

namespace uff {

/**
 * @brief The UFF ImpulseResponse class specifies a temporal impulse response
 */
struct ImpulseResponse {
 public:
  // CTOR & DTOR
  ImpulseResponse() = delete;
  explicit ImpulseResponse(double sampling_frequency, std::vector<double> data = {},
                           double time_offset = 0., std::string units = "")
      : _sampling_frequency(sampling_frequency),
        _data(std::move(data)),
        _units(std::move(units)),
        _time_offset(time_offset) {}
  ImpulseResponse(const ImpulseResponse&) = default;
  ImpulseResponse(ImpulseResponse&&) = default;
  ~ImpulseResponse() = default;

  // Operators
  ImpulseResponse& operator=(const ImpulseResponse& other) noexcept = default;
  ImpulseResponse& operator=(ImpulseResponse&& other) noexcept = default;
  inline bool operator==(const ImpulseResponse& other) const {
    return ((_time_offset == other._time_offset) &&
            (_sampling_frequency == other._sampling_frequency) && (_data == other._data) &&
            (_units == other._units));
  }
  inline bool operator!=(const ImpulseResponse& other) const { return !(*this == other); }

  // Accessors
  inline double samplingFrequency() const { return _sampling_frequency; }
  inline void setSampleFrequency(double sampling_frequency) {
    _sampling_frequency = sampling_frequency;
  }

  inline const std::vector<double>& data() const { return _data; }
  inline void setData(const std::vector<double>& data) { _data = data; }

  inline std::string units() const { return _units; }
  inline void setUnits(const std::string& units) { _units = units; }

  // Members
 private:
  // Sampling frequency in Hz
  double _sampling_frequency;

  // Collection of samples containing the impulse response
  std::vector<double> _data;

  // (Optional) Name of the units of the impulse response
  std::string _units;

  // Time offset delaying the launch of the acquisition element
  double _time_offset = 0.;
};

}  // namespace uff
