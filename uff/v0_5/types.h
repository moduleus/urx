#pragma once

/*!
 * Copyright Moduleus
 * \file uff/types.h
 * \brief
 */

#include <ostream>

namespace uff::v0_5 {

enum class WaveType {
  CONVERGING_WAVE = 0,
  DIVERGING_WAVE = 1,
  PLANE_WAVE = 2,
  CYLINDRICAL_WAVE = 3
};

std::ostream& operator<<(std::ostream& out, const WaveType& value);

}  // namespace uff::v0_5