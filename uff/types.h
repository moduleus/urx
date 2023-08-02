#pragma once

namespace uff {

enum class WaveType {
  UNDEFINED = -1,
  CONVERGING_WAVE = 0,
  DIVERGING_WAVE = 1,
  PLANE_WAVE = 2,
  CYLINDRICAL_WAVE = 3,
  PHOTOACOUSTIC_WAVE = 4
};

}  // namespace uff
