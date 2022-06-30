/*!
 * Copyright Moduleus
 * \file uff_types.h
 * \brief
 */

#ifndef UFF_TYPES_H
#define UFF_TYPES_H

namespace uff {

enum class WaveType {
  CONVERGING_WAVE = 0,
  DIVERGING_WAVE = 1,
  PLANE_WAVE = 2,
  CYLINDRICAL_WAVE = 3
};

}  // namespace uff

#endif  // UFF_TYPES_H