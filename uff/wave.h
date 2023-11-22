#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <uff/detail/compare.h>
#include <uff/excitation.h>
#include <uff/probe.h>
#include <uff/transform.h>
#include <uff/uff.h>
#include <uff/vector.h>

namespace uff {

/**
 * @brief The UFF class describing the ultrasonic wave
 */
struct Wave {
  enum class WaveType {
    CONVERGING_WAVE = 0,
    DIVERGING_WAVE = 1,
    PLANE_WAVE = 2,
    CYLINDRICAL_WAVE = 3,
    PHOTOACOUSTIC_WAVE = 4,
    UNDEFINED = -1
  };

  bool operator==(const Wave& other) const = default;

  /// Type of wave
  WaveType wave_type = WaveType::UNDEFINED;

  /// Description of the wave origin, its interpretation depends on the WaveType
  Transform origin;

  /// Time zero reference point in Probe orthogonal coordinate system
  Vector3D<double> time_zero_reference_point;

  /// Time zero for the wave [s]
  double time_zero = UFF_NAN;

  /// Maps probe elements to transmit channels.
  /// The first dimension is the probe element index, the second is an array of transmit channel indexes.
  /// When the channel_mapping is not defined it is assumed that the n-th transmit channel maps to the n-th probe element.
  /// If probe element i-th is not connected then channel_mapping[i] is empty.
  std::vector<std::vector<uint32_t>> channel_mapping;

  /// List of the excitation waveform for each channel
  std::vector<std::weak_ptr<Excitation>> channel_excitations;

  /// List of delay applied before the transmission for each channel
  std::vector<double> channel_delays;
};

}  // namespace uff
