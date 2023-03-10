#pragma once

/*!
 * Copyright Moduleus
 * \file uff/types.h
 * \brief
 */

#include <ostream>

namespace uff {

enum class WaveType {
  CONVERGING_WAVE = 0,
  DIVERGING_WAVE = 1,
  PLANE_WAVE = 2,
  CYLINDRICAL_WAVE = 3
};

std::ostream& operator<<(std::ostream& out, const WaveType& value);

}  // namespace uff