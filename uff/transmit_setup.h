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

struct TransmitSetup {
  bool operator==(const TransmitSetup& other) const {
    bool are_channel_excitations_equaled =
        channel_excitations.size() == other.channel_excitations.size();
    for (uint32_t i = 0; i < channel_excitations.size() && are_channel_excitations_equaled; ++i) {
      are_channel_excitations_equaled =
          are_channel_excitations_equaled &&
          (channel_excitations[i].expired() == other.channel_excitations[i].expired()) &&
          (channel_excitations[i].expired() ||
           (*(channel_excitations[i].lock()) == *(other.channel_excitations[i].lock())));
    }

    return ((time_offset == other.time_offset) && (probe.expired() == other.probe.expired()) &&
            (probe.expired() || (*(probe.lock()) == *(other.probe.lock()))) &&
            (origin == other.origin) && (wave_type == other.wave_type) &&
            (aperture == other.aperture) &&
            (time_zero_reference_point == other.time_zero_reference_point) &&
            (time_zero == other.time_zero) && (channel_mapping == other.channel_mapping) &&
            are_channel_excitations_equaled && (channel_delays == other.channel_delays));
  }
  inline bool operator!=(const TransmitSetup& other) const { return !(*this == other); }

  // Reference to the probe use in transmission
  std::weak_ptr<Probe> probe;

  // Geometric origin of the wave.
  Transform origin;

  // Type of wave
  WaveType wave_type = WaveType::UNDEFINED;

  // Description of the aperture used to produce the wave
  std::optional<Aperture> aperture = std::nullopt;

  // Time zero reference point in Probe orthogonal coordinate system
  Vector3D<double> time_zero_reference_point = Vector3D<double>{0, 0, 0};

  // Time zero for the wave [s]
  double time_zero = 0.;

  // Map of channels to transducer elements
  // Each value corresponds to one probe channel index
  std::vector<int> channel_mapping;

  // List of the excitation waveform for each channel
  std::vector<std::weak_ptr<Excitation>> channel_excitations;

  // List of the excitation waveform for each channel
  std::vector<double> channel_delays;

  // Time offset delaying the launch of the acquisition element
  double time_offset = 0.;
};

}  // namespace uff
