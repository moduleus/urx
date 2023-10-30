#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <uff/aperture.h>
#include <uff/excitation.h>
#include <uff/probe.h>
#include <uff/transform.h>
#include <uff/types.h>
#include <uff/uff.h>
#include <uff/vector.h>

namespace uff {

/**
 * @brief The UFF class describing the ultrasonic wave
 */
struct Wave {
  bool operator==(const Wave& other) const {
    bool are_channel_excitations_equaled =
        channel_excitations.size() == other.channel_excitations.size();
    for (uint32_t i = 0; i < channel_excitations.size() && are_channel_excitations_equaled; ++i) {
      are_channel_excitations_equaled = are_channel_excitations_equaled &&
                                        (channel_excitations[i] == other.channel_excitations[i]);
    }

    return ((origin == other.origin) && (wave_type == other.wave_type) &&
            (aperture == other.aperture) &&
            (time_zero_reference_point == other.time_zero_reference_point) &&
            (time_zero == other.time_zero) && (channel_mapping == other.channel_mapping) &&
            are_channel_excitations_equaled && (channel_delays == other.channel_delays));
  }
  inline bool operator!=(const Wave& other) const { return !(*this == other); }

  /// Type of wave
  WaveType wave_type = WaveType::UNDEFINED;

  /// Description of the wave origin, its interpretation depends on the WaveType
  Transform origin;

  /// (Optionnal) Description of the aperture used to produce the wave
  std::optional<Aperture> aperture = std::nullopt;

  /// Time zero reference point in Probe orthogonal coordinate system
  Vector3D<double> time_zero_reference_point = Vector3D<double>{0, 0, 0};

  /// Time zero for the wave [s]
  double time_zero = 0.;

  /// Map of channels to transducer elements
  /// Each value corresponds to one probe channel index
  std::vector<int32_t> channel_mapping;

  /// List of the excitation waveform for each channel
  std::vector<std::shared_ptr<Excitation>> channel_excitations;

  /// List of delay applied before the transmission for each channel
  std::vector<double> channel_delays;
};

}  // namespace uff
